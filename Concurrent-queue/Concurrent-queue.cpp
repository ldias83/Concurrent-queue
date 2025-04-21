#include <iostream>
#include <thread>
#include <atomic>
#include <array>


struct alignas(64) PaddedIndex {
	std::atomic<std::size_t> value{ 0 };
};

template<typename T, size_t N>
class RingQueue {
public:

	constexpr auto pop(T& item) -> bool {
		const auto t = m_tail.value.load(std::memory_order_relaxed);
		const auto h = m_head.value.load(std::memory_order_acquire);

		if (t == h) {
			return false;
		}

		item = m_queue[t & (N - 1)];
		m_tail.value.store(t + 1, std::memory_order_release);

		return true;
	}

	constexpr auto push(T item) -> bool {
		const auto h = m_head.value.load(std::memory_order_relaxed);
		const auto t = m_tail.value.load(std::memory_order_acquire);

		if ((h - t) == N)
			return false;

		m_queue[h & (N - 1)] = item;
		m_head.value.store(h + 1, std::memory_order_release);
		return true;
	}

private:
	std::array<T, N> m_queue;
	PaddedIndex m_head;
	PaddedIndex m_tail;
};


int main() {
	RingQueue<int, 8> q;

	std::thread prod([&] {
		for (int i = 1; i <= 20; ++i)
			while (!q.push(i));           // busy‑wait if full
		});

	std::thread cons([&] {
		int v;
		for (int i = 1; i <= 20; ++i) {
			while (!q.pop(v));
			std::cout << v << ' ';
		}
		});

	prod.join();  
	cons.join();

	return 0;
}