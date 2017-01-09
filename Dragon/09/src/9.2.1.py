import copy

# Sample BEGIN
flow_graph = {
    'ENTRY': {},
    'B1': {
        'codes': {
            1: 'i = m - 1',
            2: 'j = n',
            3: 'a = u1'
        }
    },
    'B2': {
        'codes': {
            4: 'i = i + 1',
            5: 'j = j - 1'
        }
    },
    'B3': {
        'codes': {
            6: 'a = u2'
        }
    },
    'B4': {
        'codes': {
            7: 'i = u3'
        }
    },
    'EXIT': {}
}

flow_graph['ENTRY']['output'] = ['B1']
flow_graph['B1']['output'] = ['B2']
flow_graph['B2']['output'] = ['B3', 'B4']
flow_graph['B3']['output'] = ['B4']
flow_graph['B4']['output'] = ['B2', 'EXIT']
# Sample END

flow_graph = {
    'ENTRY': {},
    'B1': {
        'codes': {
            1: 'a = 1',
            2: 'b = 2'
        }
    },
    'B2': {
        'codes': {
            3: 'c = a + b',
            4: 'd = c - a'
        }
    },
    'B3': {
        'codes': {
            5: 'd = b + d'
        }
    },
    'B4': {
        'codes': {
            6: 'd = a + b',
            7: 'e = e + 1'
        }
    },
    'B5': {
        'codes': {
            8: 'b = a + b',
            9: 'e = c - a'
        }
    },
    'B6': {
        'codes': {
            10: 'a = b * d',
            11: 'b = a - d'
        }
    },
    'EXIT': {}
}

flow_graph['ENTRY']['output'] = ['B1']
flow_graph['B1']['output'] = ['B2']
flow_graph['B2']['output'] = ['B3']
flow_graph['B3']['output'] = ['B4', 'B5']
flow_graph['B4']['output'] = ['B3']
flow_graph['B5']['output'] = ['B2', 'B6']
flow_graph['B6']['output'] = ['EXIT']

for name, node in flow_graph.items():
    if 'output' not in flow_graph[name]:
        continue
    for output in flow_graph[name]['output']:
        if 'input' not in flow_graph[output]:
            flow_graph[output]['input'] = []
        flow_graph[output]['input'].append(name)

code_len = 0
keys = []
codes = {}
for node in flow_graph.values():
    if 'codes' in node:
        for key, code in node['codes'].items():
            keys.append(key)
            codes[key] = code
            if key > code_len:
                code_len = key
keys.sort()

names = sorted(flow_graph.keys())

in_set, out_set = {}, {}
for name in flow_graph.keys():
    in_set[name] = {}
    out_set[name] = {}
    for key in keys:
        in_set[name][key] = False
        out_set[name][key] = False

changed = True
in_history, out_history = [], []
while changed:
    changed = False
    for name in names:
        node = flow_graph[name]
        out_old = copy.deepcopy(out_set[name])
        for key in keys:
            in_set[name][key] = False
            out_set[name][key] = False
        if 'input' in node:
            for in_name in node['input']:
                for key in keys:
                    if out_set[in_name][key]:
                        in_set[name][key] = True
                        out_set[name][key] = True
        if 'codes' in node:
            for cur, code in node['codes'].items():
                head = code.split(' ')[0]
                for key in keys:
                    if out_set[name][key]:
                        if head == codes[key].split(' ')[0]:
                            out_set[name][key] = False
                out_set[name][cur] = True
        for key in keys:
            if out_set[name][key] != out_old[key]:
                changed = True
    if not changed:
        break
    in_history.append(copy.deepcopy(in_set))
    out_history.append(copy.deepcopy(out_set))

table = '| Block $$B$$ | $$\\text{OUT}[B]^0$$ |'
for i in xrange(len(in_history)):
    table += ' $$\\text{IN}[B]^' + str(i + 1) + '$$ |'
    table += ' $$\\text{OUT}[B]^' + str(i + 1) + '$$ |'
table += '\n'

table += '|:-:|:-:|'
for _ in xrange(len(in_history)):
    table += ':-:|:-:|'
table += '\n'

for name in names:
    if name == 'ENTRY':
        continue
    if name == 'EXIT':
        table += '| EXIT |'
    else:
        table += '| $$B_{' + name[1:] + '}$$ |'
    table += ' '
    for key in keys:
        table += '0'
    table += ' |'
    for i in xrange(len(in_history)):
        table += ' '
        for key in keys:
            if in_history[i][name][key]:
                table += '1'
            else:
                table += '0'
        table += ' | '
        for key in keys:
            if out_history[i][name][key]:
                table += '1'
            else:
                table += '0'
        table += ' |'
    table += '\n'
print(table)
