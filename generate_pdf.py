#!/usr/bin/python
import os
import subprocess
code_dir = "../"
title = "Stanford ACM-ICPC Team Notebook"

def get_sections():
    sections = []
    section_name = None
    with open('contents.txt', 'r') as f:
        for line in f:
            if '#' in line: line = line[:line.find('#')]
            line = line.strip()
            if len(line) == 0: continue
            if line[0] == '[':
                section_name = line[1:-1]
                print(section_name)
                subsections = []
                if section_name is not None:
                    sections.append((section_name, subsections))
            else:
                tmp = line.split()
                tmp = [tmp[0], ' '.join(tmp[1:])]
                if len(tmp) == 1:
                    raise ValueError('Subsection parse error: %s' % line)
                filename = tmp[0]
                print('filename=', filename)
                subsection_name = tmp[1]
                if subsection_name is None:
                    raise ValueError('Subsection given without section')
                subsections.append((filename, subsection_name))
    return sections

def get_style(filename):
    ext = filename.lower().split('.')[-1]
    if ext in ['c', 'cc', 'cpp']:
        return 'cpp'
    elif ext in ['java']:
        return 'java'
    elif ext in ['py']:
        return 'py'
    else:
        return 'txt'

# TODO: check if this is everything we need
def texify(s):
    return s

def split_file(code_dir, filename):
    tmp_dir = 'tmp'
    os.makedirs(tmp_dir, exist_ok=True)
    content = open(os.path.join(code_dir, filename)).readlines()
    new_filename = filename.replace('/', '_')

    seps = list()
    for i in range(len(content)):
        if '---------' in content[i]:
            seps.append(i)
    if  not seps:
        seps = [-1, len(content)]
    assert(len(seps) % 2 == 0)
    new_content = list()
    print('SEPS: ', seps)
    for i in range(0, len(seps), 2):
        l = seps[i]
        r = seps[i + 1]
        print('l = ', l, ', r = ', r)
        if new_content:
            new_content.append('\n')
        for j in range(l + 1, r):
            print('j = ', j)
            new_content.append(content[j])
    new_filepath = os.path.join(tmp_dir, new_filename)
    print('NEW_CONTENT:')
    print(new_content)
    open(new_filepath, 'w').writelines(new_content)
    return new_filepath

def get_tex(sections):
    tex = []
    add = tex.append
    for (section_name, subsections) in sections:
        print(section_name)
        add('\\section{%s}\n' % texify(section_name))
        for (filename, subsection_name) in subsections:
            add('\\subsection{%s}\n' % texify(subsection_name))
            new_filename = split_file(code_dir, filename)
            add('\\raggedbottom\\lstinputlisting[style=%s]{%s}\n' % (get_style(filename), new_filename))
            add('\\hrulefill\n')
        add('\n')
    return ''.join(tex)

if __name__ == "__main__":
    sections = get_sections()
    tex = get_tex(sections)
    with open('contents.tex', 'w') as f:
        f.write(tex)
    subprocess.call(['rm', 'notebook.pdf'])
    latexmk_options = ["latexmk", "-pdf", "notebook.tex"]
    subprocess.call(latexmk_options)
