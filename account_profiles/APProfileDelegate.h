#ifndef AP_PROFILE_DELEGATE_HPP
#define AP_PROFILE_DELEGATE_HPP

namespace ap
{
	namespace delegate
	{
		class ProfileDelegate {
		public:
			virtual void onProfileSelected(int profile_id) {};
			virtual void onProfileCreated(int profile_id) {};
		};
        class ChooseProfileDelegate
        {
        public:
            virtual void onProfileProcessed(int profile_id, bool is_new) {};
        };
	}
}
#endif // !AP_PROFILE_DELEGATE_HPP
