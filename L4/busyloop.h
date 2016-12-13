#ifndef BUSYLOOP_H
#define BUSYLOOP_H

#define BILLION 1000000000
#define MILLION 1000000
#define MILLIONTH 0.000001
#define MSEC 1000

int busyLoop (int asec, int msec);
void loop (long sec, long milisec);
int gtd (struct timespec t1, struct timespec t2, long duration);

#endif
