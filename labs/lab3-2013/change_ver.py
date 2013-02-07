import sys
import shutil
import os
import re


def check_and_format_args(argv):
    if len(argv) != 3:
        print "Usage: python change_ver.py lab_number new_lab_part"
        sys.exit(1)
    else:
        return int(argv[1]), int(argv[2])


def copy_files(lab_no, lab_part):
    extentions = ["c", "frag", "vert"]
    for ext in extentions:
        infile = "lab{0}-{1}".format(lab_no, lab_part - 1)
        outfile = "lab{0}-{1}".format(lab_no, lab_part)
        copy_file(infile, outfile, ext)


def copy_file(src, dst, ext):
    src_full = src + "." + ext
    dst_full = dst + "." + ext
    if os.path.exists(dst_full):
        print "{0} already exists".format(dst_full)
        return
    try:
        src_file = open(src_full, 'r')
    except IOError:
        print "Could not open {0}".format(src_full)
        sys.exit(1)
        return

    try:
        dst_file = open(dst_full, 'w')
    except IOError:
        print("Error: Could not write to file {0}".format(dst_full))

    print "Copying file {0} to {1}".format(src_full, dst_full)
    for line in src_file:
        if ext == 'c':
            new_line = line.replace(src, dst)
            if new_line != line:
                print "Changed \n{0} to \n{1}".format(line, new_line)
            dst_file.write(new_line)
        else:
            dst_file.write(line)
    src_file.close()
    dst_file.close()


def edit_makefile(lab_no, lab_part):
    print "Editing makefile"
    this_lab = "lab{0}-{1}".format(lab_no, lab_part)
    rule_line = False
    already_made = False
    try:
        shutil.move("makefile", "makefile~")
    except IOError:
        print "makefile~ already exists"
        sys.exit(1)
    out_make = open("makefile", 'w')
    in_make = open("makefile~", 'r')
    for line in in_make:
        if already_made:
            out_make.write(line)

        elif line.startswith('all'):
            if line.endswith(this_lab):
                already_made = True
                out_make.write(line)
            else:
                out_make.write(line[:-1] + ' ' + this_lab + '\n')

        elif line.startswith('lab'):
            out_make.write(line)
            rule_line = True
            continue

        elif line.startswith('clean'):
            out_make.write("{0}: {0}.c GL_utilities.c\n".format(this_lab))
            rule = re.sub(r'lab[0-9]-[0-9]', this_lab, rule)
            out_make.write(rule + '\n')
            out_make.write(line)

        else:
            out_make.write(line)

        if rule_line:
            rule = line
            rule_line = False

    out_make.write("\trm {0}".format(this_lab))
    out_make.close()
    in_make.close()
    os.remove("makefile~")


if __name__ == "__main__":
    lab_no, lab_part = check_and_format_args(sys.argv)
    copy_files(lab_no, lab_part)
    edit_makefile(lab_no, lab_part)
