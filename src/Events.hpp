#pragma once

//struct to hold the mapData alongside the base treasure map formID in the unordered_map.
struct mapData
{
    int objectiveIndex;
    RE::FormID reference;
    const char* fileName;
    bool isContainerActivatedRef;
    const char* referenceFileName;
};

namespace Events
{
    //Offsets
    constexpr inline REL::RelocationID IsObjectiveDisplayedAddr = RELOCATION_ID(56304, 56678);
    constexpr inline REL::RelocationID SetObjectiveDisplayedAddr = RELOCATION_ID(56308, 56682);
    constexpr inline REL::RelocationID SetObjectiveCompletedAddr = RELOCATION_ID(56307, 56681);
    constexpr inline REL::RelocationID GetAliasByIDAddr = RELOCATION_ID(56364, 56723);
    constexpr inline REL::RelocationID ForceRefToAddr = RELOCATION_ID(54634, 55288);
    constexpr inline REL::RelocationID ClearAliasAddr = RELOCATION_ID(54632, 55286);

    //Filenames
    constexpr const char* DMName = "LegacyoftheDragonborn.esm";
    constexpr const char* THName = "TreasureHunter.esp";
    constexpr const char* NTName = "treasure_hunt.esp";
    constexpr const char* SKName = "Skyrim.esm";

    //Variables
    static RE::TESQuest* Quest;
    static RE::TESDataHandler* DataHandler;
    static RE::NiPoint3 DragonBridgeChestPosition;
    static RE::NiPoint3 DragonBridgeActivPosition;
    static RE::BGSListForm* CompletedTreasureMaps;
    static std::unordered_map<RE::FormID, mapData> DataMap;

    //Class
    class CHandler final :
        public RE::BSTEventSink<RE::BooksRead::Event>,
        public RE::BSTEventSink<RE::TESActivateEvent>
    {

    public:
        CHandler(CHandler const&) = delete;
        CHandler(CHandler const&&) = delete;
        CHandler operator=(CHandler&) = delete;
        CHandler operator=(CHandler&&) = delete;

        static void RegisterHandler();
        static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }
        EventResult	ProcessEvent(RE::BooksRead::Event const* a_event, RE::BSTEventSource<RE::BooksRead::Event>*) override;
        EventResult	ProcessEvent(RE::TESActivateEvent const* a_event, RE::BSTEventSource<RE::TESActivateEvent>*) override;

    private:
        CHandler() = default;
        ~CHandler() = default;

        static bool IsModInstalled(std::string_view a_modname);
        static bool ArePositionsEqual(const RE::TESObjectREFR* a, const RE::TESObjectREFR* b);
        static bool IsInSameCell(const RE::TESObjectREFR* xMarkerREFR, const RE::TESObjectREFR* containerREFR);

        static void ProcessXMarkerActivation(const mapData& a_data, const RE::TESObjectREFR* a_container);
        static void ProcessContainerActivation(const mapData& a_data, const RE::TESObjectREFR* a_container);

        static void AddTreasureMapDataToArray(const RE::FormID& a_form, const mapData& a_data);
        static void ForceActivatedReferences(const mapData& a_data);

        static void InstallMapData();
        static void InstallLOTDMapData();

        static RE::TESDataHandler* GetDataHandler();
        static std::string GetFormIDHexString(RE::FormID a_formID);

        static bool IsObjectiveDisplayed(void*, int32_t, int32_t aiObjective);
        static void SetObjectiveDisplayed(void*, int32_t, int32_t aiObjective, bool abDisplayed, bool abForce);
        static void SetObjectiveCompleted(void*, int32_t, int32_t aiObjective, bool abCompleted);
        static RE::BGSRefAlias* GetAliasByID(void*, int32_t, int32_t aiAliasID);
        static void ForceRefTo(void*, int32_t, RE::BGSRefAlias* aAlias, RE::TESObjectREFR* aReference, void*);
        static void ClearAlias(void*, int32_t, RE::BGSRefAlias* aAlias, void*);
    };

    constexpr std::pair<RE::FormID, mapData> _MapData[]
    {
        {0x0EF07A, mapData(1,  0x0F4A00, SKName)},                  //Riverwood		 					   000F49FE >> 000F4A00
        {0x0F33CE, mapData(2,  0x0F4A14, SKName)},                  //Valtheim Towers					   000F4A13 >> 000F4A14
        {0x0F33CF, mapData(3,  0x0F4A08, SKName)},                  //Solitude Lighthouse				   000F4A07 >> 000F4A08
        {0x0F33D1, mapData(4,  0x0F4A03, SKName)},                  //Pelegia Farm						   000F4A02 >> 000F4A03
        {0x0F33D4, mapData(5,  0x0F4A06, SKName)},                  //Bard's Leap Summit				   000F4A05 >> 000F4A06
        {0x0F33D0, mapData(6,  0x0F4A11, SKName)},                  //Korvanjund						   000F4A10 >> 000F4A11
        {0x0F33D5, mapData(7,  0x0F4A0E, SKName)},                  //Gallows Rock						   000F4A0D >> 000F4A0E
        {0x0F33D3, mapData(8,  0x0F4A16, SKName)},                  //Dragon Bridge						   000F4A15 >> 000F4A16
        {0x0F33CD, mapData(9,  0x0F4A0C, SKName)},                  //Broken Helm Hollow				   000F4A0B >> 000F4A0C
        {0x0F33E0, mapData(10, 0x0F4A1C, SKName)},                  //Ilinalta's Deep					   000F4A1B >> 000F4A1C
        {0x0F33D2, mapData(11, 0x0EC3D5, SKName)},                  //Fort Neugrad						   000F4A0A >> 000EC3D5
        {0x0DDEFB, mapData(12, 0x0DDEF6, SKName, true, SKName)},    //Velehk Sain's						   Do Quest >> 000DDEF6

        {0x021824, mapData(13, 0x021823, THName)},                  //Alchemist's Shack					   86021825 >> 86021823
        {0x0095BA, mapData(14, 0x008351, THName)},                  //Nightcaller Temple				   860095BB >> 86008351
        {0x00834F, mapData(15, 0x00834E, THName)},                  //Crabber's Shanty					   86008350 >> 8600834E
        {0x00834C, mapData(16, 0x007DE1, THName)},                  //Goldenglow Estate					   8600834D >> 86007DE1
        {0x00787E, mapData(17, 0x00786D, THName)},                  //Deep Folk Crossing				   8600787F >> 8600786D
        {0x006DA9, mapData(18, 0x0012C8, THName)},                  //Fort Amol							   86006DAA >> 860012C8
        {0x000D63, mapData(19, 0x00AB7B, THName)},                  //The Frozen Hearth					   860012C7 >> 8600AB7B

        {0x000803, mapData(20, 0x000817, NTName)},                  //Pale Pass							   FE1F1813 >> FE1F1817
        {0x000804, mapData(21, 0x00082C, NTName, true, NTName)},    //Gjukar's Monument					   FE1F1837 >> FE1F182C
        {0x000805, mapData(22, 0x00081A, NTName)},                  //Deep Folk Crossing				   FE1F1838 >> FE1F181A
        {0x000806, mapData(23, 0x00081B, NTName)},                  //Refugees' Rest					   FE1F1839 >> FE1F181B
        {0x000807, mapData(24, 0x00081C, NTName)},                  //karthwasten						   FE1F1815 >> FE1F181C
        {0x000808, mapData(25, 0x00081D, NTName)},                  //Wayward Pass						   FE1F1814 >> FE1F181D
        {0x000809, mapData(26, 0x00081E, NTName)},                  //Morthal							   FE1F1D6B >> FE1F181E
        {0x00080B, mapData(27, 0x00081F, NTName)},                  //Tower Stone						   FE1F183B >> FE1F181F
        {0x00080C, mapData(28, 0x000833, NTName, true, NTName)},    //Solitude Ruins					   FE1F1812 >> FE1F1833
        {0x00080D, mapData(29, 0x00082D, NTName, true, NTName)},    //Broken Tower Redoubt				   FE1F183C >> FE1F182D
        {0x00080E, mapData(30, 0x00082A, NTName, true, NTName)},    //FR: Bilegulch Ridge Dragon Mound	   FE1F1825 >> FE1F182A
        {0x000D62, mapData(31, 0x00083E, NTName)},                  //Eldergleam Sanctuary				   FE1F1D6D >> FE1F183E
        {0x000D63, mapData(32, 0x000818, NTName)},                  //Dushnikh Yal						   FE1F183A >> FE1F1818
    };                               

    constexpr std::pair<RE::FormID, mapData> _MapDataMuseum[]
    {
        {0x0EF07A, mapData(1,  0x3BF8FB, SKName, true, DMName)},    //Riverwood							   000F49FE >> 0F3BF8FB
        {0x0F33CE, mapData(2,  0x3BF91A, SKName, true, DMName)},    //Valtheim Towers					   000F4A13 >> 0F3BF91A
        {0x0F33CF, mapData(3,  0x3BF959, SKName, true, DMName)},    //Solitude Lighthouse				   000F4A07 >> 0F3BF959
        {0x0F33D1, mapData(4,  0x3BF8FD, SKName, true, DMName)},    //Pelegia Farm						   000F4A02 >> 0F3BF8FD
        {0x0F33D4, mapData(5,  0x3BF95F, SKName, true, DMName)},    //Bard's Leap Summit				   000F4A05 >> 0F3BF95F
        {0x0F33D0, mapData(6,  0x3BF8DC, SKName, true, DMName)},    //Korvanjund						   000F4A10 >> 0F3BF8DC
        {0x0F33D5, mapData(7,  0x3BF8F0, SKName, true, DMName)},    //Gallows Rock						   000F4A0D >> 0F3BF8F0
        {0x0F33D3, mapData(8,  0x0F4A16, SKName)},                  //Dragon Bridge						   000F4A15 >> 000F4A16
        {0x0F33CD, mapData(9,  0x3BF931, SKName, true, DMName)},    //Broken Helm Hollow				   000F4A0B >> 0F3BF931
        {0x0F33E0, mapData(10, 0x3BF967, SKName, true, DMName)},    //Ilinalta's Deep					   000F4A1B >> 0F3BF967
        {0x0F33D2, mapData(11, 0x3BF90F, SKName, true, DMName)},    //Fort Neugrad						   000F4A0A >> 0F3BF90F
        {0x0DDEFB, mapData(12, 0x0DDEF6, SKName, true, SKName)},    //Velehk Sain's						   Do Quest >> 000DDEF6

        {0x0EFC52, mapData(13, 0x3BF8FE, DMName, true, DMName)},    //Alchemist's Shack					   0F0F201D >> 0F3BF8FE >> <MzuhlacExterior01>
        {0x0EFC4F, mapData(14, 0x3BF8E1, DMName, true, DMName)},    //Nightcaller Temple				   0F0F218D >> 0F3BF8E1 >> <WhiteBluffHideoutExterior02>
        {0x0EFC4E, mapData(15, 0x3BF94E, DMName, true, DMName)},    //Crabber's Shanty					   0F0F210D >> 0F3BF94E >> <OrphansTearExterior>
        {0x0EFC4D, mapData(16, 0x3BF92B, DMName, true, DMName)},    //Goldenglow Estate					   0F0F219A >> 0F3BF92B >> MAKE BOOK REF PERSISTENT
        {0x0EFC4C, mapData(17, 0x3BF937, DMName, true, DMName)},    //Deep Folk Crossing				   0F0F2126 >> 0F3BF937 >> <DoomstoneTundraMarsh01>
        {0x0EFC4A, mapData(18, 0x3BF921, DMName, true, DMName)},    //Fort Amol							   0F0F297D >> 0F3BF921 >> <DBMVQBromgolt>
        {0x0EFC47, mapData(19, 0x3BF8F9, DMName, true, DMName)},    //The Frozen Hearth					   0F0F21A5 >> 0F3BF8F9 >> <POIPineForest18> 

        {0x0EFC3B, mapData(20, 0x3BF960, DMName, true, DMName)},    //Pale Pass							   0F373709 >> 0F3BF960 >> <WhiterunBanneredMare> 
        {0x0EFC3C, mapData(21, 0x0F21A3, DMName, true, DMName)},    //Gjukar's Monument					   0F0F2118 >> 0F0F21A3 >> MAKE BOOK REF PERSISTENT
        {0x0EFC3D, mapData(22, 0x3BF93E, DMName, true, DMName)},    //Deep Folk Crossing				   0F0F1F86 >> 0F3BF93E >> <POIFallForest12b> 
        {0x0EFC3E, mapData(23, 0x3BF926, DMName, true, DMName)},    //Refugees' Rest					   0F0F1EC8 >> 0F3BF926 >> <POITundra01> 
        {0x0EFC3F, mapData(24, 0x3BF946, DMName, true, DMName)},    //karthwasten						   0F0F3B37 >> 0F3BF946 >> <BonechillPassage01> 
        {0x0EFC40, mapData(25, 0x3BF8E4, DMName, true, DMName)},    //Wayward Pass						   0F0F3BF9 >> 0F3BF8E4 >> <LundsHut01> 
        {0x0EFC41, mapData(26, 0x3BF952, DMName, true, DMName)},    //Morthal							   0F0F1B93 >> 0F3BF952 >> <WindhelmHalloftheDead> 
        {0x0EFC43, mapData(27, 0x3BF8EC, DMName, true, DMName)},    //Tower Stone						   0F0F20E9 >> 0F3BF8EC >> <SteamcragCampExterior01> 
        {0x0EFC44, mapData(28, 0x0F2111, DMName, true, DMName)},    //Solitude Ruins					   0F0F3BFA >> 0F0F2111 >> <MarkarthArnleifandSonsTradingCompany> 
        {0x0EFC45, mapData(29, 0x0F2107, DMName, true, DMName)},    //Broken Tower Redoubt				   0F0F21AB >> 0F0F2107 >> <RiftenOrigin>
        {0x0EFC46, mapData(30, 0x0F21A0, DMName, true, DMName)},    //FR: Bilegulch Ridge Dragon Mound	   0F0F219B >> 0F0F21A0 >> MAKE BOOK REF PERSISTENT
        {0x0EFC39, mapData(31, 0x3BF968, DMName, true, DMName)},    //Eldergleam Sanctuary				   0F2FE89C >> 0F3BF968 >> MAKE BOOK REF PERSISTENT
        {0x0EFC3A, mapData(32, 0x3BF93F, DMName, true, DMName)},    //Dushnikh Yal						   0F37370A >> 0F3BF93F >> MAKE BOOK REF PERSISTENT

        {0x470D2A, mapData(33, 0x470D36, DMName, true, DMName)},    //Autumnshade clearing				   0F0F201D >> 0F470D36 >> Add Book To Inventory
        {0x470D2B, mapData(34, 0x470D38, DMName, true, DMName)},    //Labyrinthian						   0F0F218D >> 0F470D38 >> Add Book To Inventory
        {0x470D2C, mapData(35, 0x470D3A, DMName, true, DMName)},    //Alftand							   0F0F210D >> 0F470D3A >> Add Book To Inventory
        {0x470D2D, mapData(36, 0x470D3C, DMName, true, DMName)},    //Shimmermist Cave					   0F0F219A >> 0F470D3C >> Add Book To Inventory
        {0x470D2E, mapData(37, 0x470D3E, DMName, true, DMName)},    //Silent Moons Camp					   0F0F2126 >> 0F470D3E >> Add Book To Inventory
        {0x470D2F, mapData(38, 0x470D40, DMName, true, DMName)},    //Dawnstar Shrine					   0F0F297D >> 0F470D40 >> Add Book To Inventory
        {0x470D30, mapData(39, 0x470D42, DMName, true, DMName)},    //Shriekwind Bastion				   0F0F21A5 >> 0F470D42 >> Add Book To Inventory
        {0x470D31, mapData(40, 0x470D44, DMName, true, DMName)},    //Bannermist Tower					   0F0F21A5 >> 0F470D44 >> Add Book To Inventory
        {0x470D32, mapData(41, 0x470D46, DMName, true, DMName)},    //Soljund's Sinkhole				   0F0F21A5 >> 0F470D46 >> Add Book To Inventory
        {0x470D33, mapData(42, 0x470D4E, DMName, true, DMName)},    //Golden Glow Estate				   0F0F21A5 >> 0F470D4E >> Add Book To Inventory
        {0x600AF9, mapData(43, 0x600B00, DMName, true, DMName)},    //Dragon Bridge					       0F0F297D >> 0F600B00 >> Add Book To Inventory
        {0x600AFA, mapData(44, 0x600B01, DMName, true, DMName)},    //Karthwasten						   0F0F21A5 >> 0F600B01 >> Add Book To Inventory
        {0x600AFB, mapData(45, 0x600B02, DMName, true, DMName)},    //Angi's Camp					       0F0F21A5 >> 0F600B02 >> Add Book To Inventory
        {0x600AFC, mapData(46, 0x600B03, DMName, true, DMName)},    //Rift Imperial Camp				   0F0F21A5 >> 0F600B03 >> Add Book To Inventory
        {0x600AFD, mapData(47, 0x600B04, DMName, true, DMName)},    //Gjukar's Monument				       0F0F21A5 >> 0F600B04 >> Add Book To Inventory
    };
}