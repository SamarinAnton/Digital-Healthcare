import matplotlib.pyplot as plt


def show_speedups(y, x, title, x_label):
    plt.plot(x, [d[0] for d in y], color='red', label='SPMD')
    plt.plot(x, [d[1] for d in y], color='green', label='OpenMP')
    plt.plot(x, [d[2] for d in y], color='magenta', label='OpenMP with hack')

    plt.xlabel(x_label)
    plt.ylabel('Speedup')
    plt.title(title)
    plt.legend(loc='best')
    plt.grid()
    plt.show()


def show_absolute(y, x, title, x_label):
    plt.plot(x, [d[0] for d in y], color='red', label='serial')
    plt.plot(x, [d[1] for d in y], color='green', label='SPMD')
    plt.plot(x, [d[2] for d in y], color='magenta', label='OpenMP')
    plt.plot(x, [d[3] for d in y], color='blue', label='OpenMP with hack')

    plt.xlabel(x_label)
    plt.ylabel('Time, seconds')
    plt.title(title)
    plt.legend(loc='best')
    plt.grid()
    plt.show()


with open('size_data.txt', 'r') as size_data_file:
    matrices_data = [int(x.strip()) for x in size_data_file]

with open('number_threads.txt', 'r') as number_threads_file:
    number_threads = [int(x.strip()) for x in number_threads_file]

with open('time_threads.txt', 'r') as time_threads_file:
    speedups_threads = []
    time_threads = []

    for data in time_threads_file:
        number_times = [float(x) for x in data.strip().split(',') if x != '']
        serial_threads, *speedup_threads = number_times
        speedups_threads.append(speedup_threads)
        time_threads.append([serial_threads, serial_threads / speedup_threads[0],
                             serial_threads / speedup_threads[1], serial_threads / speedup_threads[2]])

with open('time_size.txt', 'r') as time_size_file:
    speedups_size = []
    time_size = []

    for data in time_size_file:
        number_times = [float(x) for x in data.strip().split(',') if x != '']
        serial_size, *speedup_size = number_times
        speedups_size.append(speedup_size)
        time_size.append([serial_size, serial_size / speedup_size[0],
                          serial_size / speedup_size[1], serial_size / speedup_size[2]])


show_absolute(time_size, matrices_data, 'Dependence time from size data', 'Size matrices, N * N')
show_absolute(time_threads, number_threads, 'Dependence time from number of threads', 'Number of threads')
show_speedups(speedups_size, matrices_data, 'Dependence speedup from size data', 'Size matrices, N * N')
show_speedups(speedups_threads, number_threads, 'Dependence speedup from number of threads', 'Number of threads')
