import getopt
import sys
import helper
import menu
import logging


def usage():
    print("Help")


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
    output = None
    for opt, arg in options:
        if opt in ('-v', '--version'):
            # Outputs the git token of the commit
            output += helper.get_gittoken()
        elif opt in ('-h', '--help'):
            # Help on possible parameters
            output += usage()
        elif opt in ('-o', '--output'):
            # Outputs to file with FILENAME instead of the console
            output_file = arg
        elif opt in ('-p', '--python'):
            # Outputs python-environment, then exits
            output_file = "Python Version: " + helper.python_info()
            sys.exit()
        elif opt in ('-s', '--system'):
            # Outputs system info, then exits
            output_file = menu.pretty_print(helper.system_info())
            sys.exit()
        elif opt in ('-c', '--cpu'):
            # Outputs CPU-info, then exits
            output_file = menu.pretty_print_nested(helper.cpu_info())
            sys.exit()
        elif opt in ('-m', '--memory'):
            # Summarizes all the entries in /proc/[pid]/statm on all listed
            # entries about running tasks
            break
        if

if __name__ == "__main__":
    main()
