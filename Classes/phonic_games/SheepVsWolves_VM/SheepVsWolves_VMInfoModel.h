#ifndef MJ_GAME_SHEEP_VS_WOLVES_VM_INFO_MODEL_H_
#define MJ_GAME_SHEEP_VS_WOLVES_VM_INFO_MODEL_H_

#include <string>
#include <vector>

namespace SheepVsWolvesVM {
	struct BOARD_INFO {
		std::string text_full;
		std::string audio_full;
		std::string image_path;
		std::string video_path;
	};
	struct PHONIC_INFO {
		std::string text_phonic;
		std::string audio_phonic;
		bool missing;
	};
	struct SYNC_TEXT_INFO {
		std::string text;
		std::string audio;
		std::vector<int> list_index;
	};
	struct GAME_INFO
	{
		std::string text_full;
		std::string n_audio;
		std::string p_audio;
        std::string text_khong_dau;
        std::string audio_khong_dau;
        std::string text_dau;
        std::string audio_dau;
        bool co_dau = false;
        int pos_dau = 0;
//        std::string s_audio;
		std::vector<PHONIC_INFO> list_phonics;
		BOARD_INFO board_info;
		std::vector<PHONIC_INFO> list_distracting;
		std::vector<SYNC_TEXT_INFO> list_synctext_info;
	};
}

#endif // !MJ_GAME_SHEEP_VS_WOLVES_INFO_MODEL_H_
