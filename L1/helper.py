import platform
from collections import OrderedDict
import subprocess
import shlex
import os
import resource
import logging

logging.basicConfig(filename='mygetopt.log', level=logging.INFO)


pagesize = resource.getpagesize()  # Size of the page needed to convert to MB


def python_info():
    '''
    :return: Python-Versions-String (ohne Leerzeichen)
    '''
    return platform.python_version()


def system_info():
    '''
    :return: Dictionary mit entspr. Systeminformationen
    '''

    os = platform.system()

    if os == 'Darwin':
        return OrderedDict([("LABEL", "CONTENT"),
                            ("Architecture", platform.architecture()[0]),
                            ("Kernel Release", platform.release()),
                            ("Real Processor", platform.processor()),
                            ("System", os),
                            ("Mac Release", platform.mac_ver()[0])])
    elif os == 'Linux':
        return OrderedDict([("LABEL", "CONTENT"),
                            ("Architecture", platform.architecture()[0]),
                            ("Kernel Release", platform.release()),
                            ("System", os),
                            # deprecated functions
                            ("Linux Release",
                             platform.linux_distribution()[1]),
                            ("Linux Distribution", platform.linux_distribution()[0])])
    elif os == 'Windows':
        pass
    else:
        pass


def codename(model, family):
    '''
    :param model:
    :param family:
    :return: Den Codenamen als String
    '''
    model = str("0x{:02x}".format(model))
    family = str("0x{:02x}".format(family))

    file = open("proc_code.txt", 'r')

    for line in file.readlines():
        if model and family in line:
            # slices the codename from line
            return line.split(':')[0].replace(" ", "")


def cpu_info():
    '''
    :return: Dictionary mit Dictionary, Key des ersten Dictionaries
    ist die CPU Nummer (z.B. 1), das zweite Dictionary enthaelt somit das
    Dictionary mit folgenden CPU Informationen
    '''

    os = platform.system()

    if os == 'Darwin':
        model = int(
            subprocess.check_output(shlex.split("sysctl -n machdep.cpu.model")))
        family = int(
            subprocess.check_output(shlex.split("sysctl -n machdep.cpu.family")))
        frequency = str(int(subprocess.check_output(
            shlex.split("sysctl -n hw.cpufrequency"))) * (10**-6)) + " Mhz"
        modelname = str(
            subprocess.check_output(shlex.split("sysctl -n machdep.cpu.brand_string")))
        modelname = modelname[2:len(modelname)-3]  # remove string modifiers
        cm = codename(model, family)

        return OrderedDict([('Modelname', modelname),
                            ('Architecture', platform.architecture()[0]),
                            ('Model', str(model)),
                            ('Family', str(family)),
                            ('Frequency', frequency),
                            ('Codename', cm)])
    elif os == "Linux":
        cpuinfo = get_proc_cpuinfo_as_list()
        cpus = int(subprocess.check_output(
            shlex.split("grep -c ^processor /proc/cpuinfo ")))  # number of cpu
        file = open("/proc/cpuinfo", "r")
        d = OrderedDict()
        while(len(cpuinfo) > 0):
            for i in range(cpus):
                d[str(i)] = OrderedDict()
                for j in range(5):
                    d[str(i)][cpuinfo[0][0]] = cpuinfo[0][1]
                    del cpuinfo[0]

        # add additional attributes which are not in the cpuinfo file
        for key in d.keys():
            d[key]["architecture"] = platform.architecture()[0]
            d[key]["codename"] = codename(
                model=int(d[key]["model"]), family=int(d[key]["cpu family"]))
        return d

    elif os == 'Windows':
        pass


def get_proc_cpuinfo_as_list():
    '''
    :return: double list with processor number,
    model/name, cpu family and cpufrequency
    '''
    infos = subprocess.check_output(shlex.split(
        "grep 'processor\|model name\|cpu family\|model\|cpu MHz' /proc/cpuinfo ")).decode(encoding="UTF-8")
    infos = infos.replace("\\", "").replace("\t", "").split("\n")

    for item in range(len(infos)):
        infos[item] = infos[item].split(":")

    del infos[len(infos)-1]  # remove empty entry
    return infos


def get_gittoken():
    '''
    :return: The git token of the current commit as a string.

    Returns and error if no git revision was found.
    '''
    try:
        git_token = subprocess.check_output(
            ["git", "rev-parse", "--short", "HEAD"])
        return git_token.strip()
    except subprocess.CalledProcessError:
        git_token = "No git revision was found. Please create a repository."
        logging.error()


def memory_sum():
    '''
    :return: A dictionary with the summed properties of
        all the statm entries in /proc/.

    Goes through all the folders (including subfolders) and
    adds up all the sizes.
    '''
    size = 0
    resident = 0
    share = 0
    text = 0
    total = 0
    data = 0
    for root, dirs, files in os.walk('/proc/'):
        for file in files:
            if file.endswith('statm'):
                statm = open(os.path.join(root, file))
                stats = [int(i) for i in statm.read().split()]

                size += stats[0] / pagesize
                resident += stats[1] / pagesize
                share += stats[2] / pagesize
                text += stats[3] / pagesize
                data += stats[5] / pagesize
            total = (size + resident + share + text + data)
    return OrderedDict([('size', str(round(size, 2)) + " MB"),
                        ('resident', str(round(resident, 2)) + " MB"),
                        ('share', str(round(share, 2)) + " MB"),
                        ('text', str(round(text, 2)) + " MB"),
                        ('SUM', str(round(total, 2)) + " MB")])


def memory_top(arg):
    '''
    :param arg: Argument from the command line that is the category
    :return: The top ten list of memory users in that category

    Goes through all the task folders (excluding task folders)
    and creates the list.
    '''
    memory_list = []
    for root, dirs, files in os.walk('/proc/'):
        for file in files:
            # Don't visit subtasks folders
            if 'task' in dirs:
                dirs.remove('task')
            if file.endswith('statm'):
                statm = open(os.path.join(root, file))
                stats = [int(i) for i in statm.read().split()]
                if(arg == 'size'):
                    size = round(stats[0] / pagesize, 2)
                elif (arg == 'resident'):
                    size = round(stats[1] / pagesize, 2)
                elif (arg == 'share'):
                    size = round(stats[2] / pagesize, 2)
                elif (arg == 'text'):
                    size = round(stats[3] / pagesize, 2)

                # Gets the process name from the comm file
                process_name_file = open(os.path.join(root, 'comm'))
                process_name = process_name_file.read()

                memory_list.append((process_name.strip(), size))
    return (sorted(memory_list, key=lambda tup: tup[1], reverse=True)[:10])
