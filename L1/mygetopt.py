import getopt
import sys
import helper
import menu
import logging


def usage():
    return """Usage: mygetopt.py [OPTION]

         -v, --version      Outputs the git token of the commit
         -h, --help         Displays this help
         -o, --output       Sends the output the the specified file
         -p, --python       Displays the python environment
         -s, --system       Displays system info
         -c, --cpu          Displays CPU info
         -m, --memory       Shows memory information"""


def print_list(list):
    i = 1
    output = ""
    for name, size in list:
        output += "{}.\t{}\t{} MB\n".format(i, name, size)
        i = i + 1
    return output


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
            output += "\n" + str(helper.get_gittoken()) + "\n"
        elif opt in ('-h', '--help'):
            # Help on possible parameters
            output += "\n" + usage() + "\n"
        elif opt in ('-o', '--output'):
            # Outputs to file with FILENAME instead of the console
            output_file = open(arg, 'w')
        elif opt in ('-p', '--python'):
            # Outputs python-environment, then exits
            output += "\n" + "Python Version: " + helper.python_info() + "\n"
            # sys.exit()
        elif opt in ('-s', '--system'):
            # Outputs system info, then exits
            output += "\n" + menu.pretty_print(helper.system_info()) + "\n"
            # sys.exit()
        elif opt in ('-c', '--cpu'):
            # Outputs CPU-info, then exits
            output += "\n" + menu.pretty_print_nested(helper.cpu_info()) + "\n"
            # sys.exit()
        elif opt in ('-m', '--memory'):
            # Summarizes all the entries in /proc/[pid]/statm on all listed
            # entries about running tasks
            if(arg == 'all'):
                output += "\n" + menu.pretty_print(helper.memory_sum()) + "\n"
            elif arg in ('size', 'resident', 'share', 'text'):
                output += "\n" + print_list(helper.memory_top(arg)[:10])
            else:
                output += "\nNot a valid argument.\n" + usage() + "\n"

    if(output_file is None):
        print(output.expandtabs(30)[1:-1])
    else:
        output_file.write(output.expandtabs(30)[1:-1])

if __name__ == "__main__":
    main()
