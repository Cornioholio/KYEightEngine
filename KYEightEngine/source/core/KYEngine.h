#pragma once

namespace KYEight 
{
	class KYEngine
	{
	public:
		KYEngine();
		~KYEngine();

		KYEngine(const KYEngine&) = delete;
		KYEngine& operator=(const KYEngine&) = delete;

		KYEngine(const KYEngine&&) = delete;
		KYEngine& operator=(KYEngine&&) = delete;

		bool Initialize();
		void Run();
		void Shutdown();

	private:
	};
}