REGISTER_NUM = 2


def init_ershov(codes, t=0):
    if codes['op'] == 'leaf':
        codes['ershov'] = 1
        return t
    t = init_ershov(codes['lc'], t)
    l = codes['lc']['ershov']
    if 'rc' in codes:
        t = init_ershov(codes['rc'], t)
        r = codes['rc']['ershov']
        if l == r:
            codes['ershov'] = l + 1
        else:
            codes['ershov'] = max(l, r)
    else:
        codes['ershov'] = l
    t += 1
    codes['t'] = t
    return t


def gen_codes(codes, b=0):
    if 'ershov' not in codes:
        init_ershov(codes)
    k = codes['ershov']
    if codes['op'] == 'leaf':
        print('LD  R' + str(b + 1) + ', ' + codes['lc'])
    elif 'rc' not in codes:
        gen_codes(codes['lc'], b)
        if codes['op'] == '-':
            print('SUB R' + str(b + k) + ', #0, R' + str(b + k))
        elif codes['op'] == '*':
            print('ST  R' + str(b + k) + ', *R' + str(b + k))
    else:
        op = {
            '+': 'ADD',
            '-': 'SUB',
            '*': 'MUL',
            '/': 'DIV'
        }[codes['op']]
        le = codes['lc']['ershov']
        re = codes['rc']['ershov']
        if k <= REGISTER_NUM - b:
            if le == re:
                gen_codes(codes['rc'], b + 1)
                gen_codes(codes['lc'], b)
                a, b, c = b + k, b + k - 1, b + k
            elif le < re:
                gen_codes(codes['rc'], b)
                gen_codes(codes['lc'], b)
                a, b, c = b + k, b + le, b + k
            else:
                gen_codes(codes['lc'], b)
                gen_codes(codes['rc'], b)
                a, b, c = b + k, b + k, b + re
            print(op + ' R' + str(a) + ', R' + str(b) + ', R' + str(c))
        else:
            if le <= re:
                t = codes['rc']['t']
                gen_codes(codes['rc'], b)
                print('ST  t' + str(t) + ', R' + str(REGISTER_NUM))
                gen_codes(codes['lc'], REGISTER_NUM - re)
                print('LD  R' + str(REGISTER_NUM - 1) + ', t' + str(t))
                a, b, c = REGISTER_NUM, REGISTER_NUM, REGISTER_NUM - 1
            else:
                t = codes['lc']['t']
                gen_codes(codes['lc'], b)
                print('ST  t' + str(t) + ', R' + str(REGISTER_NUM))
                gen_codes(codes['rc'], REGISTER_NUM - le)
                print('LD  R' + str(REGISTER_NUM - 1) + ', t' + str(t))
            print(op + ' R' + str(a) + ', R' + str(b) + ', R' + str(c))


if __name__ == '__main__':
    codes_0 = {
        'op': '+',
        'lc': {
            'op': '-',
            'lc': {
                'op': 'leaf',
                'lc': 'a'
            },
            'rc': {
                'op': 'leaf',
                'lc': 'b',
            }
        },
        'rc': {
            'op': '*',
            'lc': {
                'op': 'leaf',
                'lc': 'e'
            },
            'rc': {
                'op': '+',
                'lc': {
                    'op': 'leaf',
                    'lc': 'c'
                },
                'rc': {
                    'op': 'leaf',
                    'lc': 'd'
                }
            }
        }
    }
    codes_1 = {
        'op': '-',
        'lc': {
            'op': '/',
            'lc': {
                'op': 'leaf',
                'lc': 'a'
            },
            'rc': {
                'op': '+',
                'lc': {
                    'op': 'leaf',
                    'lc': 'b'
                },
                'rc': {
                    'op': 'leaf',
                    'lc': 'c'
                }
            }
        },
        'rc': {
            'op': '*',
            'lc': {
                'op': 'leaf',
                'lc': 'd'
            },
            'rc': {
                'op': '+',
                'lc': {
                    'op': 'leaf',
                    'lc': 'e'
                },
                'rc': {
                    'op': 'leaf',
                    'lc': 'f'
                }
            }
        }
    }
    codes_2 = {
        'op': '+',
        'lc': {
            'op': 'leaf',
            'lc': 'a'
        },
        'rc': {
            'op': '*',
            'lc': {
                'op': 'leaf',
                'lc': 'b'
            },
            'rc': {
                'op': '*',
                'lc': {
                    'op': 'leaf',
                    'lc': 'c'
                },
                'rc': {
                    'op': '*',
                    'lc': {
                        'op': 'leaf',
                        'lc': 'd'
                    },
                    'rc': {
                        'op': 'leaf',
                        'lc': 'e'
                    }
                }
            }
        }
    }
    codes_3 = {
        'op': '*',
        'lc': {
            'op': '+',
            'lc': {
                'op': '-',
                'lc': {
                    'op': 'leaf',
                    'lc': 'a'
                }
            },
            'rc': {
                'op': '*',
                'lc': {
                    'op': 'leaf',
                    'lc': 'p'
                }
            }
        },
        'rc': {
            'op': '/',
            'lc': {
                'op': '-',
                'lc': {
                    'op': 'leaf',
                    'lc': 'b'
                },
                'rc': {
                    'op': '*',
                    'lc': {
                        'op': 'leaf',
                        'lc': 'q'
                    }
                }
            },
            'rc': {
                'op': '+',
                'lc': {
                    'op': '-',
                    'lc': {
                        'op': 'leaf',
                        'lc': 'c'
                    }
                },
                'rc': {
                    'op': '*',
                    'lc': {
                        'op': 'leaf',
                        'lc': 'r'
                    }
                }
            }
        }
    }
    gen_codes(codes_3)
