#include "cbos.h"
#include "utils.h"

/**
 * Main benchmarking function. It monitors the encryption process over time,
 * calculates CPU cycles used in the encryption process and evaluates performance
 * metrics, including the average bytes per cycle, variance, and standard deviation.
 *
 * @param crypto_library Pointer to the cryptographic library and ciphers.
 * @param message_size Size of the message to encrypt.
 * @param iterations Number of benchmark iterations.
 *
 * @return True if the benchmark succeeds; otherwise, false.
 */
bool benchmark(const Crypto *crypto_library, const size_t message_size, const size_t iterations)
{
	bool ok = true;

	if (!crypto_library)
	{
		fprintf(stderr, "Error: crypto library is not provided.\n");
		return !ok;
	}
	else if (iterations <= 0)
	{
		fprintf(stderr, "Error: The number of iterations must be greater than zero.\n");
		return !ok;
	}

	const char *name = crypto_library->name();

	void *cipher_parameters = NULL;
	if (!crypto_library->init(&cipher_parameters))
	{
		printf("Error: [%s] cipher parameters initialization failed!\n", name);
		return !ok;
	}

	uint8_t *src = malloc(message_size);
	uint8_t *dst = malloc(message_size);

	if (!crypto_library->random(cipher_parameters, message_size, src))
	{
		printf("Error: [%s] input randomization failed!\n", name);
	}

	const char **ciphers = crypto_library->ciphers();
	for (size_t i = 0; ciphers[i] != NULL; ++i)
	{
		const char *cipher = ciphers[i];
		if (!crypto_library->set_cipher(cipher_parameters, cipher))
		{
			printf("Error: [%s] failed to set %s, skipping it...\n", name, cipher);
			continue;
		}

		// Create a progress thread to monitor the benchmark progress
		pthread_t progress_thread;
		Progress progress;
		progress.iterations_total = iterations;
		progress.lib_name = name;

		printf("[%s] running %s benchmark...\n", name, cipher);

		pthread_create(&progress_thread, NULL, progress_function, &progress);

		const double start = seconds();
		progress.start_time = start;

		// Perform encryption for the specified number of iterations
		for (size_t i = 0; i < iterations; ++i)
		{
			size_t ret = crypto_library->encrypt(cipher_parameters, message_size, dst, src);
			if (!ret)
			{
				printf("Error: [%s] encryption failed!\n", name);
				ok = false;
				break;
			}
			progress.iterations_completed = i;
		}

		const double elapsed = seconds() - start;

		pthread_join(progress_thread, NULL);
		printf("[%s] %f seconds for %zu iterations, %zu bytes message\n", name, elapsed, iterations, message_size);

		// Measure the performance of a computation process.
		double bytes_per_cycle[iterations];
		long cycles_start, cycles_end, cycles_used;
		double total_bytes_per_cycle = 0.0;

#ifdef __aarch64__
		ccnt_init();
#endif

		for (int test = 0; test < iterations; ++test)
		{

			cycles_start = timestamp();
			size_t ret = crypto_library->encrypt(cipher_parameters, message_size, dst, src);
			cycles_end = timestamp();
			if (!ret)
			{
				printf("Error: [%s] encryption failed!\n", name);
				ok = false;
				break;
			}

			cycles_used = cycles_end - cycles_start;

			// Calculate bytes/cycle for this test round
			bytes_per_cycle[test] = (double)message_size / (double)cycles_used;
			total_bytes_per_cycle += (double)bytes_per_cycle[test];
		}

		double variance = 0.0;
		double average_bytes_per_cycle = total_bytes_per_cycle / iterations;

		for (int i = 0; i < iterations; ++i)
		{
			variance += pow(bytes_per_cycle[i] - average_bytes_per_cycle, 2);
		}
		variance /= iterations;

		double std_deviation = sqrt(variance);

		printf("[%s] Average Bytes/cycle count: %lf \n", name, average_bytes_per_cycle);
		printf("[%s] Variance: %lf\n", name, variance);
		printf("[%s] Standard Deviation: %lf\n", name, std_deviation);
	}

	crypto_library->free(cipher_parameters);

	free(src);
	free(dst);

	return ok;
}

int main()
{

	bool ok = true;

	if (!benchmark(get_lib(), get_message_size(), get_iterations()))
	{
		ok = false;
	}
	return !ok;
}