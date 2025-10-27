import matplotlib.pyplot as plt

x = [1e6, 1e7, 1e8]
str_1 = [5.27, 44.92, 244.11]
str_2 = [6.10, 29.50, 323.13]
str_3 = [6.35, 31.19, 328.41]

plt.plot(x, str_1, marker='o', label='str_1')
plt.plot(x, str_2, marker='o', label='str_2')
plt.plot(x, str_3, marker='o', label='str_3')

plt.xscale('log')
plt.xlabel('# de consultas')
plt.ylabel('Tiempo (ms)')
plt.title('Benchmarks para la operación wavelet_kth_smallest')
plt.legend()
plt.grid(True)

plt.show()

