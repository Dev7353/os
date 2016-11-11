import getopt
import os
import sys
from collections import OrderedDict
from prettytable import PrettyTable


def pretty_print(d):
    output = ""
    for key in d.keys():
        output += key + "\t" + str(d[key]) + "\n"
    output = output[:-1]
    return output


def get_env():
    try:
        return OrderedDict([("CWD:", os.getcwd()), ("Home:", os.environ['HOME']),
                            ("PWD:", os.path.abspath(
                                os.path.join(os.getcwd(), os.pardir))),
                            ("Path:", os.environ[
                             'PATH']), ("Shell:", os.environ['SHELL']),
                            ("User:", os.environ['USER']
                             ), ("UID:", os.getuid()),
                            ("GID:", os.getgid()), ("VEnv:", os.environ['VIRTUAL_ENV'])])
    except (KeyError, OSError, AttributeError):
        print("A mistake occured. Likely a key wasn't found.")


def get_tree():
    dict = OrderedDict()
    for root, dirs, files in os.walk('/proc/' + str(os.getpid())):
        for file in files:
            if 'task' in dirs:
                dirs.remove('task')
            if file.endswith('comm'):
                process_name_file = open(os.path.join(root, 'comm'))
                process_name = process_name_file.read()
                stat = open(os.path.join(root, 'stat')).readline()
                status = stat.split(' ')[2]
                dict.update({'0': (os.getpid(), process_name.strip(), status)})
    # dict.update({'1': (os.getppid(), ' ', ' ')})
    current_pid = os.getpid()
    i = 1
    while(current_pid != 0):
        parent_pid = os.popen('ps -o ppid= -p ' + str(current_pid)).readline()
        current_pid = int(parent_pid)
        for root, dirs, files in os.walk('/proc/' + str(parent_pid).strip()):
            for file in files:
                if 'task' in dirs:
                    dirs.remove('task')
                if file.endswith('comm'):
                    process_name_file = open(os.path.join(root, 'comm'))
                    process_name = process_name_file.read()
                    stat = open(os.path.join(root, 'stat')).readline()
                    status = stat.split(' ')[2]
                    dict.update({str(i): (parent_pid.strip(),
                                          process_name.strip(), status)})
        i = i + 1
    return dict


def usage():
    '''
    :return: The help string
    '''
    return """Usage: mygetopt.py [OPTION]

         -v, --version      Outputs the git token of the commit
         -h, --help         Displays this help
         -o, --output       Sends the output the the specified file
         -p, --python       Displays the python environment
         -s, --system       Displays system info
         -c, --cpu          Displays CPU info
         -m, --memory       Shows memory information on selected category.
                            Allowed arguments are all, size, resident,
                            share and text."""


def print_tree(d):
    x = PrettyTable(["PID", "Name", "Status"])
    x.align["PID"] = "l"  # Left align city names
    # One space between column edges and contents (default)
    x.padding_width = 1
    for i in range(0, len(d)):
        values = list(d.values())[i]
        x.add_row([values[0], values[1], values[2]])
    print(x)


def main():
    try:
        options, args = getopt.getopt(sys.argv[1:], 'ep', ['env', 'pidtree'])
    except getopt.GetoptError as err:
        # print help information and exit:
        print(err)  # will print something like "option -a not recognized"
        usage()
        sys.exit(2)
    output = ""
    if not options:
        output += "\nNo arguments found.\n" + usage() + "\n"
    for opt, arg in options:
        if opt in ('-e', '--env'):
            # Dosomething
            output += pretty_print(get_env())
        elif opt in ('-p', '--pidtree'):
            # Dosomethingelse
            print_tree(get_tree())
    print(output)

if __name__ == "__main__":
    main()
