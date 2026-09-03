#include "Events.hpp"
#undef AddForm

namespace Events
{
	void CHandler::RegisterHandler() 
	{
		//Register Quest Script
		Quest = GetDataHandler()->LookupForm<RE::TESQuest>(0x000D61, "MapMarkerTreasure.esp");

		if (!Quest) {
			ERROR("Unable To Find Marked Treasure NG Quest!")
		}

		//Obtain Completed Treasure Maps List.
		CompletedTreasureMaps = GetDataHandler()->LookupForm<RE::BGSListForm>(0x000800, "MapMarkerTreasure.esp");

		if (!CompletedTreasureMaps) {
			ERROR("Unable To Find Marked Treasure NG Formlist!")
		}

		//Register Books Read Event
		RE::BooksRead::GetEventSource()->AddEventSink(GetSingleton());

		//Register Activate Events
		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(static_cast<RE::BSTEventSink<RE::TESActivateEvent>*>(GetSingleton()));

		//Install Treasure Map Data
		IsModInstalled(DMName) ? InstallLOTDMapData() : InstallMapData();
	};

	//---------------------------------------------------
	//-- Framework Functions ( Get Data Handler ) -------
	//---------------------------------------------------

	RE::TESDataHandler* CHandler::GetDataHandler()
	{
		if (DataHandler) { return DataHandler; }

		const auto& Handler = RE::TESDataHandler::GetSingleton();
		if (!Handler) { ERROR("Unable To Obtain Data Handler!") };

		DataHandler = Handler;
		return DataHandler;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Get FormID String ) ------
	//---------------------------------------------------

	std::string CHandler::GetFormIDHexString(RE::FormID a_formID)
	{
		return std::format("{:08X}", a_formID);
	}

	//---------------------------------------------------
	//-- Framework Functions ( On Book Read ) -----------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::BooksRead::Event const* a_event, RE::BSTEventSource<RE::BooksRead::Event>*)
	{
		if (!a_event || !a_event->book || a_event->book->IsRead() || DataMap.find(a_event->book->GetFormID()) == DataMap.end())
		{
			return EventResult::kContinue;
		};

		const auto& data = DataMap.at(a_event->book->GetFormID());

		ForceActivatedReferences(data);
		SetObjectiveDisplayed(nullptr, 0, data.objectiveIndex, true, false);
		return EventResult::kContinue;
	};

	//---------------------------------------------------
	//-- Framework Functions ( On Activate ) ------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::TESActivateEvent const* a_event, RE::BSTEventSource<RE::TESActivateEvent>*)
	{
		// If there is no object activated or no reference then return.
		if (!a_event->objectActivated || !a_event->objectActivated.get() || a_event->actionRef->GetFormID() != RE::PlayerCharacter::GetSingleton()->GetFormID()) {
			return EventResult::kContinue;
		}

		auto* activatedObject = a_event->objectActivated.get();
		if (!activatedObject || !activatedObject->GetBaseObject())
		{
			return EventResult::kContinue;
		}

		// Check if the activated object's base type is not a Container
		if (activatedObject->GetBaseObject()->GetFormType() != RE::FormType::Container) {
			return EventResult::kContinue;
		}

		for (auto& [formID, data] : DataMap) 
		{
			if (data.isContainerActivatedRef) {
				ProcessContainerActivation(data, activatedObject);
				continue;
			}

			ProcessXMarkerActivation(data, activatedObject);
			continue;
		}
		return EventResult::kContinue;
	};

	//---------------------------------------------------
	//-- Framework Functions ( Is Mod Installed ) -------
	//---------------------------------------------------

	bool CHandler::IsModInstalled(std::string_view a_modname)
	{
		auto* file1 = GetDataHandler()->LookupModByName(a_modname);
		auto* file2 = GetDataHandler()->LookupLoadedModByName(a_modname);
		auto* file3 = GetDataHandler()->LookupLoadedLightModByName(a_modname);
		return file1 || file2 || file3;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Non-Standard ) ---
	//---------------------------------------------------

	void CHandler::ProcessContainerActivation(const mapData& a_data, const RE::TESObjectREFR* a_container)
	{
		auto* container = GetDataHandler()->LookupForm<RE::TESObjectREFR>(a_data.reference, a_data.referenceFileName);

		if (a_container && container && container->GetFormID() == a_container->GetFormID() && !CompletedTreasureMaps->HasForm(container)) {
			CompletedTreasureMaps->AddForm(container);
			SetObjectiveCompleted(nullptr, 0, a_data.objectiveIndex, true);
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process XMarker Act ) ----
	//---------------------------------------------------

	void CHandler::ProcessXMarkerActivation(const mapData& a_data, const RE::TESObjectREFR* a_container)
	{
		auto* xMarker = GetDataHandler()->LookupForm<RE::TESObjectREFR>(a_data.reference, a_data.fileName);

		if (a_container && xMarker && IsInSameCell(xMarker, a_container) && ArePositionsEqual(xMarker, a_container) && !CompletedTreasureMaps->HasForm(xMarker)) {
			CompletedTreasureMaps->AddForm(xMarker);
			SetObjectiveCompleted(nullptr, 0, a_data.objectiveIndex, true);
		}
	}
	
	//---------------------------------------------------
	//-- Framework Functions ( In Same Cell As Player ) -
	//---------------------------------------------------

	bool CHandler::IsInSameCell(const RE::TESObjectREFR* xMarkerREFR, const RE::TESObjectREFR* containerREFR) {
		return xMarkerREFR && containerREFR && xMarkerREFR->GetParentCell() == containerREFR->GetParentCell();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Container Position ) -----
	//---------------------------------------------------

	bool CHandler::ArePositionsEqual(const RE::TESObjectREFR* a, const RE::TESObjectREFR* b)
	{
		INFO("Checking Positions of {} : {}", GetFormIDHexString(a->GetFormID()), GetFormIDHexString(b->GetFormID()));
		RE::NiPoint3 posA = a->GetPosition();
		RE::NiPoint3 posB = b->GetPosition();

		INFO("PosA - X =  {}", posA.x);
		INFO("PosA - Y =  {}", posA.y);
		INFO("PosA - Z =  {}", posA.z);

		INFO("PosB - X =  {}", posB.x);
		INFO("PosB - Y =  {}", posB.y);
		INFO("PosB - Z =  {}", posB.z);

		if (posA == DragonBridgeActivPosition && posB == DragonBridgeChestPosition) {
			return true;
		}

		// Calculate the distance between the two positions
		float dx = posA.x - posB.x;
		float dy = posA.y - posB.y;
		float dz = posA.z - posB.z;
		float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
		INFO("Relative Distance = {}", distance);

		return distance <= 1.0f || ((posA.x == posB.x) && (posA.y == posB.y) && (posA.z == posB.z));
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install References ) -----
	//---------------------------------------------------

	void CHandler::ForceActivatedReferences(const mapData& a_data)
	{
		try {
			const std::string lookupName = a_data.referenceFileName
				? std::string(a_data.referenceFileName)
				: std::string(a_data.fileName);

			auto* reference = GetDataHandler()->LookupForm<RE::TESObjectREFR>(a_data.reference, lookupName);
			if (!reference)
			{
				INFO("Unable to force treasure map reference [{}] from: {}, unable to locate reference.", GetFormIDHexString(a_data.reference), lookupName);
				return;
			}

			auto* alias = GetAliasByID(nullptr, 0, a_data.objectiveIndex);
			if (!alias)
			{
				INFO("Unable to force treasure map reference [{}] from: {}, unable to locate alias {}.", GetFormIDHexString(a_data.reference), lookupName, a_data.objectiveIndex);
				return;
			}

			ForceRefTo(nullptr, 0, alias, reference, nullptr);
		}
		catch (const std::exception& e) {
			ERROR("Exception caught while forcing activated references: {}", e.what());
		}
		catch (...) {
			ERROR("Unknown exception caught while forcing activated references.");
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Mod Data ) -------
	//---------------------------------------------------

	void CHandler::AddTreasureMapDataToArray(const RE::FormID &a_formID, const mapData& a_data)
	{
		if (auto* form = GetDataHandler()->LookupForm(a_formID, a_data.fileName))
		{
			INFO("Got Treasure Map - [{}] from {}", form->GetName(), a_data.fileName);

			DataMap.emplace(form->GetFormID(), a_data);
			return;
		}
		INFO("Unable To Install Treasure Map [{}] From: {}, Unable to locate form.", GetFormIDHexString(a_formID), a_data.fileName);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Map Data ) -------
	//---------------------------------------------------

	void CHandler::InstallMapData() 
	{
		//Handle Dragon Bridge Container Positions
		DragonBridgeActivPosition = GetDataHandler()->LookupForm<RE::TESObjectREFR>(0x0F4A16, SKName)->GetPosition();
		DragonBridgeChestPosition = GetDataHandler()->LookupForm<RE::TESObjectREFR>(0x10FE3F, SKName)->GetPosition();

		for (auto i = 0; i < std::extent_v<decltype(_MapData)>; i++)
		{
			AddTreasureMapDataToArray(_MapData[i].first, _MapData[i].second);
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Map Data ) -------
	//---------------------------------------------------

	void CHandler::InstallLOTDMapData()
	{
		//Handle Dragon Bridge Container Positions
		DragonBridgeActivPosition = GetDataHandler()->LookupForm<RE::TESObjectREFR>(0x0F4A16, SKName)->GetPosition();
		DragonBridgeChestPosition = GetDataHandler()->LookupForm<RE::TESObjectREFR>(0x10FE3F, SKName)->GetPosition();

		for (auto i = 0; i < std::extent_v<decltype(_MapDataMuseum)>; i++)
		{
			AddTreasureMapDataToArray(_MapDataMuseum[i].first, _MapDataMuseum[i].second);
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Objective Manipulation ) -
	//---------------------------------------------------

	bool CHandler::IsObjectiveDisplayed(void*, int32_t, int32_t aiObjective)
	{
		using func_t = bool (*)(void*, int32_t, RE::TESQuest*, int32_t);
		REL::Relocation<func_t> func{ IsObjectiveDisplayedAddr };
		return func(nullptr, 0, Quest, aiObjective);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Objective Manipulation ) -
	//---------------------------------------------------

	void CHandler::SetObjectiveDisplayed(void*, int32_t, int32_t aiObjective, bool abDisplayed, bool abForce)
	{
		if (!IsObjectiveDisplayed(nullptr, 0, aiObjective))
		{
			using func_t = void (*)(void*, int32_t, RE::TESQuest*, int32_t, bool, bool);
			REL::Relocation<func_t> func{ SetObjectiveDisplayedAddr };
			func(nullptr, 0, Quest, aiObjective, abDisplayed, abForce);
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Objective Manipulation ) -
	//---------------------------------------------------

	void CHandler::SetObjectiveCompleted(void*, int32_t, int32_t aiObjective, bool abCompleted)
	{
		if (IsObjectiveDisplayed(nullptr, 0, aiObjective))
		{
			using func_t = void (*)(void*, int32_t, RE::TESQuest*, int32_t, bool);
			REL::Relocation<func_t> func{ SetObjectiveCompletedAddr };
			func(nullptr, 0, Quest, aiObjective, abCompleted);
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Objective Manipulation ) -
	//---------------------------------------------------

	RE::BGSRefAlias* CHandler::GetAliasByID(void*, int32_t, int32_t aiAliasID)
	{
		using func_t = RE::BGSRefAlias* (*)(void*, int32_t, RE::TESQuest*, int32_t);
		REL::Relocation<func_t> func{ GetAliasByIDAddr };
		return func(nullptr, 0, Quest, aiAliasID);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Objective Manipulation ) -
	//---------------------------------------------------

	void CHandler::ForceRefTo(void*, int32_t, RE::BGSRefAlias* aAlias, RE::TESObjectREFR* aReference, void*)
	{
		try {
			if (aAlias->GetReference()) 
			{
				INFO("Alias [{}] is not empty. Clearing alias before forcing reference [{}].", aAlias->aliasID, GetFormIDHexString(aReference->GetFormID()));
				ClearAlias(nullptr, 0, aAlias, nullptr);

				if (aAlias->GetReference()) 
				{
					ERROR("Failed to clear alias [{}]. Alias still holds a reference [{}].", aAlias->aliasID, GetFormIDHexString(aAlias->GetReference()->GetFormID()));
					return;
				}
				INFO("Alias [{}] successfully cleared.", aAlias->aliasID);
			}

			using func_t = decltype(ForceRefTo);
			REL::Relocation<func_t> func{ ForceRefToAddr };
			func(nullptr, 0, aAlias, aReference, nullptr);

			if (!aAlias->GetReference()) 
			{
				ERROR("Failed to fill alias [{}] with reference [{}].", aAlias->aliasID, GetFormIDHexString(aReference->GetFormID()));
				return;
			}

			INFO("Successfully forced reference [{}] to alias [{}].", GetFormIDHexString(aAlias->GetReference()->GetFormID()), aAlias->aliasID);

		}
		catch (const std::exception& e) 
		{
			ERROR("Exception caught in ForceRefTo: {}", e.what());
		}
		catch (...) 
		{
			ERROR("Unknown exception occurred in ForceRefTo.");
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Objective Manipulation ) -
	//---------------------------------------------------

	void CHandler::ClearAlias(void*, int32_t, RE::BGSRefAlias* aAlias, void*)
	{
		using func_t = decltype(ClearAlias);
		REL::Relocation<func_t> func{ ClearAliasAddr };
		func(nullptr, 0, aAlias, nullptr);
	}
};