import getopt
import sys
import helper
import menu
import logging


def usage():
    return """Usage: mygetopt.py [OPTION]\n
         -v, --version      Outputs the git token of the commit
         -h, --help         Outputs the git token of the commit
         -o, --output       Outputs the git token of the commit
         -p, --python       Outputs the git token of the commit
         -s, --system       Outputs the git token of the commit
         -c, --cpu          Outputs the git token of the commit
         -m, --memory       Outputs the git token of the commit"""


def main():
    try:
        options, args = getopt.getopt(sys.argv[1:], 'vho:pscm:', ['version',
                                                                  'help',
                                                                  'output=',
                                                                  'python',
                                                                  'system',
                                                                  'cpu',
                                                                  'memory='])
    except getopt.GetoptError as err:
        # print help information and exit:
        print(err)  # will print something like "option -a not recognized"
        usage()
        sys.exit(2)
    output = ""
    output_file = None
    for opt, arg in options:
        if opt in ('-v', '--version'):
            # Outputs the git token of the commit
            output += str(helper.get_gittoken()) + "\n"
        elif opt in ('-h', '--help'):
            # Help on possible parameters
            output += usage()
        elif opt in ('-o', '--output'):
            # Outputs to file with FILENAME instead of the console
            output_file = open(arg, 'w')
        elif opt in ('-p', '--python'):
            # Outputs python-environment, then exits
            output += "Python Version: " + helper.python_info() + "\n"
            # sys.exit()
        elif opt in ('-s', '--system'):
            # Outputs system info, then exits
            output += menu.pretty_print(helper.system_info()) + "\n"
            # sys.exit()
        elif opt in ('-c', '--cpu'):
            # Outputs CPU-info, then exits
            output += menu.pretty_print_nested(helper.cpu_info()) + "\n"
            # sys.exit()
        elif opt in ('-m', '--memory'):
            # Summarizes all the entries in /proc/[pid]/statm on all listed
            # entries about running tasks
            if(arg == 'all'):
                output += menu.pretty_print(helper.memory_sum()) + "\n"

    if(output_file is None):
        print(output.expandtabs(30))
    else:
        output_file.write(output.expandtabs(30))

if __name__ == "__main__":
    main()
