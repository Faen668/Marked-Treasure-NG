#include "Events.hpp"

const SKSE::MessagingInterface* g_messaging = nullptr;
const SKSE::LoadInterface* g_LoadInterface = nullptr;
const SKSE::QueryInterface* g_QueryInterface = nullptr;

static void SKSEMessageHandler(SKSE::MessagingInterface::Message* message)
{
	auto t1 = std::chrono::steady_clock::now();

	switch (message->type)
	{
	case SKSE::MessagingInterface::kDataLoaded:
		//Events::CHandler::RegisterHandler();
		break;

	case SKSE::MessagingInterface::kNewGame:
		Events::CHandler::RegisterHandler();
		break;

	case SKSE::MessagingInterface::kPostLoadGame:
		Events::CHandler::RegisterHandler();
		break;

	default:
		break;
	}
}

DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
#ifndef NDEBUG
	while (!IsDebuggerPresent()) { Sleep(100); }
#endif

	DKUtil::Logger::Init(Plugin::NAME, REL::Module::get().version().string());
	SKSE::Init(a_skse);

	INFO("{} v{} loaded", Plugin::NAME, Plugin::Version);

	g_messaging = reinterpret_cast<SKSE::MessagingInterface*>(a_skse->QueryInterface(SKSE::LoadInterface::kMessaging));
	if (!g_messaging)
	{
		INFO("Failed to load messaging interface! This error is fatal, plugin will not load.");
		return false;
	}

	SKSE::Init(a_skse);
	SKSE::AllocTrampoline(256);

	g_messaging->RegisterListener("SKSE", SKSEMessageHandler);
	return true;
}