REGISTER_NUM = 2

registers = None
addresses = None
generated = None

register_num = 0
last_line = 0


def parse_code(raw):
    code = raw.split(' ')
    if len(code) == 5:
        parsed = {
            'target': code[0],
            'operands': [code[2], code[4]],
            'source': raw
        }
        if code[3] == '+':
            parsed['type'] = '+'
        elif code[3] == '-':
            parsed['type'] = '-'
        elif code[3] == '*':
            parsed['type'] = '*'
        elif code[3] == '/':
            parsed['type'] = '/'
        return parsed
    if len(code) == 4:
        if code[0] != '*':
            return {
                'type': '=*',
                'target': code[0],
                'operands': [code[4]],
                'source': raw
            }
        return {
            'type': '*=',
            'target': code[0],
            'operands': [code[4]],
            'source': raw
        }
    if '[' in code[0]:
        return {
            'type': '[]=',
            'target': code[0].split('[')[0],
            'operands': [code[0].split('[')[1].split(']')[0], code[2]],
            'source': raw
        }
    if '[' in code[2]:
        return {
            'type': '=[]',
            'target': code[0],
            'operands': [code[2].split('[')[0], code[2].split('[')[1].split(']')[0]],
            'source': raw
        }
    return {
        'type': '=',
        'target': code[0],
        'operands': [code[2]],
        'source': raw
    }


def load(reg, name):
    generated.append('LD  R' + str(reg + 1) + ', ' + name)
    registers[reg] = [name]
    if reg not in addresses[name]:
        addresses[name].append(reg)


def store(name, reg):
    if name not in addresses[name]:
        generated.append('ST  ' + name + ', R' + str(reg + 1))
        addresses[name].append(name)


def gen_operand_reg(operand_name, follows, operands=[]):
    global addresses
    global register_num
    for address in addresses[operand_name]:
        if isinstance(address, int):
            return address
    for i in xrange(REGISTER_NUM):
        if len(registers[i]) == 0:
            if i >= register_num:
                register_num = i + 1
            return i
    for i in xrange(REGISTER_NUM):
        if i in operands:
            continue
        is_valid = True
        for name in registers[i]:
            if len(filter(lambda x: x != i, addresses[name])) == 0:
                is_valid = False
                break
        if is_valid:
            return i
    for i in xrange(REGISTER_NUM):
        if i in operands:
            continue
        is_valid = True
        for name in registers[i]:
            for code in follows:
                if name in code['operands']:
                    is_valid = False
                    break
        if is_valid:
            return i
    for i in xrange(REGISTER_NUM):
        if i in operands:
            continue
        for name in registers[i]:
            store(name, i)
        return i
    raise Exception('Not enough registers: ' + operand_name)


def gen_target_reg(target_name, follows, operands=[]):
    global addresses
    global register_num
    for i in xrange(REGISTER_NUM):
        if len(registers[i]) == 1:
            if registers[i][0] == target_name:
                return i
    for i in xrange(REGISTER_NUM):
        if len(registers[i]) == 0:
            if i >= register_num:
                register_num = i + 1
            return i
    for i in xrange(REGISTER_NUM):
        if i in operands:
            continue
        is_valid = True
        for name in registers[i]:
            if len(filter(lambda x: x != i, addresses[name])) == 0:
                is_valid = False
                break
        if is_valid:
            return i
    for operand in operands:
        if isinstance(operand, int):
            if len(registers[operand]) == 1:
                name = registers[operand][0]
                is_valid = True
                for code in follows:
                    if name in code['operands']:
                        is_valid = False
                        break
                if is_valid:
                    return operand
    for i in xrange(REGISTER_NUM):
        if i in operands:
            for name in registers[i]:
                store(name, i)
            return i
    raise Exception('Not enough registers: ' + target_name)


def addresses_to_str(adds):
    strs = []
    for add in adds:
        if isinstance(add, int):
            strs.append('R' + str(add + 1))
        else:
            strs.append(add)
    if len(strs) > 0:
        return '`' + ','.join(sorted(strs)) + '`'
    return ''


def output_detail(title):
    global last_line
    print('* ' + title + '\n')
    if last_line < len(generated):
        print('Generated: \n')
        print('```')
        while last_line < len(generated):
            print(generated[last_line])
            last_line += 1
        print('```\n')
    print('Registers: \n')
    output = '|'
    for i in xrange(register_num):
        output += ' R' + str(i + 1) + ' |'
    output += '\n|'
    for i in xrange(register_num):
        output += ':-:|'
    output += '\n|'
    for i in xrange(register_num):
        output += ' ' + addresses_to_str(registers[i]) + ' |'
    print(output)
    print('\nAddresses: \n')
    keys = sorted(addresses.keys())
    output = '|'
    for key in keys:
        output += ' `' + key + '` |'
    output += '\n|'
    for key in keys:
        output += ':-:|'
    output += '\n|'
    for key in keys:
        output += ' ' + addresses_to_str(addresses[key]) + ' |'
    print(output)
    print('')


def gen_code(codes):
    global registers
    global addresses
    global generated
    registers = [[] for _ in xrange(REGISTER_NUM)]
    addresses = {}
    codes = map(parse_code, codes)
    for code in codes:
        addresses[code['target']] = []
        for operand in code['operands']:
            if '0' <= operand[0] <= '9':
                continue
            addresses[operand] = []
    for name in addresses.keys():
        if name[0] != 't':
            addresses[name] = [name]
    generated = []
    output_detail('Entry: ')
    for index, code in enumerate(codes):
        if code['type'] in ['+', '-', '*', '/']:
            x = code['target']
            y = code['operands'][0]
            z = code['operands'][1]
            if '0' <= y[0] <= '9':
                y = '#' + y
                y_operands = [y]
            else:
                ry = gen_operand_reg(y, codes[index + 1:])
                y_operands = [y, ry]
                if y not in registers[ry]:
                    load(ry, y)
            if '0' <= z[0] <= '9':
                z = '#' + z
                z_operands = y_operands + [z]
            else:
                rz = gen_operand_reg(z, codes[index + 1:], y_operands)
                if z not in registers[rz]:
                    load(rz, z)
                z_operands = y_operands + [z, rz]
            rx = gen_target_reg(x, codes[index + 1:], z_operands)
            operator = ''
            if code['type'] == '+':
                operator = 'ADD'
            elif code['type'] == '-':
                operator = 'SUB'
            elif code['type'] == '*':
                operator = 'MUL'
            elif code['type'] == '/':
                operator = 'DIV'
            gen = operator + ' ' + 'R' + str(rx + 1) + ', '
            if y[0] == '#':
                gen += y + ', '
            else:
                gen += 'R' + str(ry + 1) + ', '
            if z[0] == '#':
                gen += z
            else:
                gen += 'R' + str(rz + 1)
            generated.append(gen)
            for key, val in addresses.items():
                addresses[key] = filter(lambda x: x != rx, val)
            registers[rx] = [x]
            addresses[x] = [rx]
        elif code['type'] == '=[]':
            x = code['target']
            a = code['operands'][0]
            i = code['operands'][1]
            ri = gen_operand_reg(i, codes[index + 1:])
            if i not in registers[ri]:
                load(ri, i)
            rx = gen_target_reg(x, codes[index + 1:], [i, ri])
            registers[rx] = [x]
            addresses[x] = [rx]
            generated.append('ST  ' + 'R' + str(rx + 1) + ', ' + a + '(R' + str(ri + 1) + ')')
        elif code['type'] == '[]=':
            a = code['target']
            i = code['operands'][0]
            x = code['operands'][1]
            ri = gen_operand_reg(i, codes[index + 1:])
            if i not in registers[ri]:
                load(ri, i)
            rx = gen_operand_reg(x, codes[index + 1:], [i, ri])
            if x not in registers[rx]:
                load(rx, x)
            registers[rx].append(a + '(R' + str(ri + 1) + ')')
            addresses[a + '(R' + str(ri + 1) + ')'] = [rx]
        elif code['type'] == '=':
            x = code['target']
            y = code['operands'][0]
            ry = gen_operand_reg(y, codes[index + 1:])
            if y not in registers[ry]:
                load(ry, y)
            registers[ry].append(x)
            addresses[x] = [ry]
        output_detail('`' + code['source'] + '`')
    for name in addresses.keys():
        if name[0] == 't':
            continue
        if name not in addresses[name]:
            for address in addresses[name]:
                if isinstance(address, int):
                    store(name, address)
                    break
    output_detail('Exit: ')


if __name__ == '__main__':
    codes = [
        't1 = i * n',
        't2 = t1 + k',
        't3 = t2 * 4',
        't4 = b[t3]',
        't5 = k * n',
        't6 = t5 + j',
        't7 = t6 * 4',
        't8 = c[t7]',
        't9 = t4 + t8',
        't10 = i * n',
        't11 = t10 + j',
        't12 = t11 * 4',
        'a[t12] = t9'
    ]
    register_num = 2
    gen_code(codes)
    print('* Complete: \n')
    print('```')
    for code in generated:
        print(code)
    print('```\n')
