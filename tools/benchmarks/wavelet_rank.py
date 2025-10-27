import matplotlib.pyplot as plt

x = [1e6, 1e7, 1e8]
str_1 = [10.34, 72.87, 526.43]
str_2 = [12.29, 75.32, 656.41]
str_3 = [12.30, 63.15, 649.74]

plt.plot(x, str_1, marker='o', label='str_1')
plt.plot(x, str_2, marker='o', label='str_2')
plt.plot(x, str_3, marker='o', label='str_3')

plt.xscale('log')
plt.xlabel('# de consultas')
plt.ylabel('Tiempo (ms)')
plt.title('Benchmarks para la operación wavelet_rank')
plt.legend()
plt.grid(True)

plt.show()
