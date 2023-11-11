//
//  MSGradeManager.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/28/17.
//

#ifndef MSGradeManager_hpp
#define MSGradeManager_hpp

#include <string>
#include "MJStoryInfoModel.h"

namespace ms {
    class GradeManager {
    public:
        
        enum Scope {
            STORIES,
            LESSONS,
            LESSONS_PHONIC,
            AUDIOBOOKS, 
            AISPEAKING,
        };
        
        static GradeManager & getInstance() {
            static GradeManager gradeManager;
            return gradeManager;
        }
        
        bool isGradeLocked(int idx);
        
        int getCurrentGradeId();
        void setCurrentGradeId(int idx);
        
        std::string getGradeNameOfId(int idx);
        
        void setInitialGrade(std::string grade_name);
        
        CC_SYNTHESIZE(Scope, _scope, Scope);
        
        std::vector<mj::model::GradeInfo> getGradesInfo();
        
        mj::model::GradeInfo getGradeInfoById(int grade_id);
        
        void updateGrades();
        
        std::string getNameByLanguage(std::string name, int lang_id);
        
        int getGradeIdByLevelId(int level_id);

        std::vector<std::string> getSubStringsInSeparatedVerticalBarString(std::string i_input);

		std::vector<int> getPhonicGradeIDs () { return phonic_grade_id; }

    protected:
        explicit GradeManager();
        
    private:
        GradeManager(GradeManager const &);
        void operator = (GradeManager const &);
//        bool isPhonic(int grade_id);
        bool isGrade(int grade_id, std::vector<int> grade_list);
        std::string getPhonicGradeName(std::string name, int lang_id);
        std::map<int, int> _map_level_to_grade;
		std::vector<int> phonic_grade_id =
		{
			469, 479, 480, 481
		};
        std::vector<mj::model::GradeInfo> _old_grades;
        std::vector<mj::model::GradeInfo> _new_grades;
    };
}

#endif /* MSGradeManager_hpp */
