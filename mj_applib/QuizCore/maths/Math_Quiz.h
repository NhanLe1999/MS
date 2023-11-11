//
//  Math_Quiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/14/17.
//
//

#ifndef Math_Quiz_h
#define Math_Quiz_h

#include <stdio.h>
#include "ui/CocosGUI.h"
#include "CocoStudioMacro.h"
#include "PoolManager.h"

#include "MJGame.h"
#include "AudioHelper.h"
#include "QuizGuideCenter.h"

#include "NumberUtils.h"
#include "Math_Func.h"

#ifndef __func__
#define __func__ __FUNCTION__
#endif

class Math_Quiz : public cocos2d::ui::Layout
{
public:
    Math_Quiz(){};
    ~Math_Quiz(){};
    
    /**
     create quiz method
     
     @return Math_Quiz object
     */
    static Math_Quiz * createQuiz();
    
    //---->Quiz Life-cycle
    /**
     Khởi tạo quizz.
     Đọc file json config, câu hỏi và chọn ảnh.
     Khởi tạo các thành phần của quiz.
     
     @param game game info
     */
    virtual void initQuiz(mj::model::Game game);
    
    /**
     Bắt đầu quiz.
     Setup các thành phần lên man hình.
     Bắt đầu câu hỏi đàu tiên.
     */
    virtual void onStart();
    
    /**
     Bắt đầu câu hỏi.
     Gọi mỗi khi 1 câu hỏi được hỏi.
     Setup những điều cần cho câu hỏi hiện tại.
     Có thể là thay đổi UI, hướng dẫn chơi ...
     */
    virtual void startQuestion();
    
    /**
     Giải thích dáp án.
     Phần tự động giải thích câu trả lời.
     Điều hướng: Chơi lại nếu trả lời sai, chuyển sang câu hỏi tiếp theo hoặc kết thúc quiz.
     
     @param correct đáp án
     */
    virtual void onExplanation(bool correct);
    
    /**
     Hoàn thành quiz.
     Remove các thành phần của quiz.
     Kết thúc và call hệ thông chuyển quiz tiếp theo.
     */
    virtual void onComplete();
    
    /**
     Chế độ test tự động.
     Không cho người chơi tương tác với quiz.
     Tự động chọn đáp án.
     */
    virtual void autoTesting();
    //----> End Quiz Life-cycle
    
    /**
     Goi khi băt dau guide
     */
    virtual void onGuideStarted();
    
    /**
     Gui khi guide xong
     */
    virtual void onGuideDone();
    
    void startGuide();
    
    
    /**
     Setup hàm callback sau khi hoàn thành quiz
     
     @param callback function gọi khi kêt thúc
     */
    void setCompleteCallback(math::callback::onCompleteQuiz callback);
    
#pragma mark - Game info
    mj::model::Game getGame();
    void setDebugText(std::string text);
    
    
#pragma mark - Control
    /**
     Animation khi hiển thị
     */
    void onShow();
    
    /**
     Animatiion khi kêt thúc

     @param cleanup xóa hoàn toàn hay không
     @return thời gian khi remove xong
     */
    virtual float onHide(bool cleanup = false);
    
    /**
     Chặn tất cả các hành vi chạm lên màn hình quiz

     @param time thời gian hiệu lực
     */
    void disableControlQuiz(float time = 0);
    
    /**
     Bỏ chặn chạm lên màn hình
     */
    void enableControlQuiz();
    
    /**
     Goi hàm khi hết thời gian chơi thử.

     @param std::function<void hảm xử lý khi kết thúc chơi thử
     */
    void setStopTrialCallback(std::function<void()> function, bool completed_stop = true);

    
    /**
     Multi voice: mỗi giọng đọc có id khác nhau

     @param vid voice id
     */
    void setVoiceId(int vid);
    
    /**
    Màn hình chúc mừng khi hoàn thành quiz

     @param dt thời gian delay
     @return thời gian kết thúc animation
     */
    CC_DEPRECATED_ATTRIBUTE float showConratulation(float dt);
    
    void quizBegan(float delay, bool show_question);
    void quizEnded(float delay, bool show_question);

protected:
#pragma mark -
    bool _initQuiz();

    std::vector<Math_Object*>   _objects;
    std::vector<Math_Object*>   _dragers;
    std::vector<Math_Object*>   _color_pens;
    std::vector<Math_Group*>    _groups;
    std::vector<Math_Slot*>     _slots;
    std::vector<Math_Answer*>   _answers;
    Math_Question*              _question = nullptr;
    
    Math_Answer* done = nullptr;
    Math_Answer* trash = nullptr;

    mj::model::Game     _game;
    std::string         _game_path = "";
    std::string         image_prefix = "";
    math::callback::onCompleteQuiz _complete_callback = nullptr;
    std::function<void ()> _stop_trial_callback = nullptr;

    cocos2d::ui::Layout* _panel_move;
    cocos2d::ui::Layout* _panel;
    cocos2d::ui::Layout* _panel_right;
    cocos2d::ui::Layout* _panel_bot;
    std::vector<cocos2d::ui::Widget*> ui_elements;

#pragma mark - Animation + Effect
    //void startQuiz
    
#pragma mark - Question
    /**
     Đọc câu hỏi

     @param loop lặp lại mẫu câu hỏi nhiều lần
     @param loop_number số lần lặp
     */
    void parseQuestion(bool loop = false, int loop_number = 1);
    void parseQuestionWithLoops(std::vector<int> loops);

    //CC_DEPRECATED_ATTRIBUTE void setQuestion(Math_Question* question);
    Math_Question* getQuestion();
    
    int questionIndex() {
        return getQuestion()->getData().index;
    }
    
    float questionHeight () {
        return getQuestion()->getContentSize().height;
    }
    
    /**
     Hiển thị câu hỏi. Bắt dầu 1 quiz

     @param dt thời gian delay
     @param ::animation::SlideDown kiểu animation xuất hiện
     */
    void showQuestionBox(float dt = 0);
    
    /**
     Kết thúc và ẩn câu hỏi

     @param dt thời gian delay
     @param ::animation::SlideDown kiểu animation kết thúc (ngươcj lại với animation xuất hiện)
     */
    void hideQuestionBox(float dt = 0);
    
    /**
     Chuyển sang câu hỏi kế tiếp theo thứ tự. Nếu không còn câu hỏi nào, kết thúc quiz.
     */
    void nextQuestion(float delay = 0);
    
    /**
     Chuyển câu hỏi đên vị trí 'index'

     @param index vị trí câu hoir chuyển đến
     */
    void goToQuestionAtIndex(int index, float delay = 0);
        
#pragma mark - UI Display
    /**
     Set background image

     @param path Đường dẫn background theo theme
     */
    void setBackGroundQuiz(std::string path);
    
    /**
     Add A child into _panel layout

     @param object đối tượng được addChild
     @param zorder position Z
     */
    void addChildNode(Node *object, int zorder=1);
    
    /**
     Add A child into _panel_right layout
     
     @param object đối tượng được addChild
     @param zorder position Z
     */
    void addChildNodeToRightPanel(Node *object, int zorder=1);
    
    /**
     Add A child into _panel_bot layout
     
     @param object đối tượng được addChild
     @param zorder position Z
     */
    void addChildNodeToBottomPanel(Node *object, int zorder=1);
    
    /**
     Hiển thị khung tương tác bên phải

     @param delay thời gian delay
     @param width độ róngo với quiz_size
     */
    void onShowRightBar(float delay = 0, float width = 0.25, math::callback::onAnswerClicked callback_done = nullptr);
    
    /**
     Đóng khung tương tác phải

     @param delay thời gian delay
     @return thời gian hoàn tất
     */
    float onHideRightBar(float delay = 0);
    
    /**
     Hiện khung tương tác dưới

     @param delay thoi gian delay
     @param height kích thước so với quiz_size
     @param sub_frame Hiển thị viền phụ
     @param callback_done Call khi click done
     @param callback_trash Call khi bâm trash
     */
    void onShowBottomBar(float delay = 0, float height = 0.25, bool sub_frame = true, math::callback::onAnswerClicked callback_done = nullptr, math::callback::onAnswerClicked callback_trash = nullptr, bool done_center = false);
    
    /**
     Đóng khung tương tác dưới

     @param delay thời gian delay
     @return thời gian hoàn tất
     */
    float onHideBottomBar(float delay = 0);
    
    /**
     Hiển thị khung tương tác phải dạng bảng chọn màu

     @param colors Danh sách các màu chọn lựa
     @param callback Hàm callback khi chọn màu
     @param callback_done Hàm callback khi bấm nút 'done'
     @param delay thời gian delay
     */
    float showColorsPad (std::vector<std::string> colors, math::callback::onTouchObjectBegan callback, math::callback::onAnswerClicked callback_done, float delay = 0, bool is_right = true);
    
    /**
     Đóng khung tương tác phải dạng bảng chọn màu

     @param delay thời gian delay
     @return thời gian hoàn tất
     */
    float hideColorsPad(float delay = 0);
    
#pragma mark - Audio
    /**
     Phát audio câu hỏi

     @param stop dừng đọc câu hỏi và phát lại
     */
    void speakQuestion(bool stop = false);
    
    /**
     Dừng đọc câu hỏi
     */
    void stopSpeakQuestion();
    
    /**
     Play audio bject

     @param object object được chạy audio
     @param key thuôc tính phát audio
     */
    CC_DEPRECATED_ATTRIBUTE void playObjectAudio(Math_Object* object, std::string key = "name_1");
    
#pragma mark - Get images
    
    math::resource::Image* groupBG(std::string color = "brown");
    math::resource::Image* objectBG();
    math::resource::Image* slotBG();
    
    /**
     Lấy Image các từ đặc biêt như [yes, no, done, ...]. Mục đích sau này k cần sửa code trong từng quiz khi chuyển gói ngôn ngữ.
6
     @param text_group Text trong 'text group'
     @return Text theo  ngôn ngữ
     */
    math::resource::Image* getTextByTextGroup(std::string text_group);
    
    /**
     Lấy danh sách màu.

     @param name loc theo tên
     @return danh sách màu
     */
    std::vector<math::resource::Image*> getColors();
    math::resource::Image* getColor(std::string name);
    /**
     Lấy danh sách số trong phạm vi. Lấy trong list ảnh, nếu ngoai list anh. tự sinh data.

     @param from Số băt đầu
     @param to Số kết thúc
     @return Danh sách số
     */
    std::vector<math::resource::Image*> getNumbers(int from, int to, math::lang::MathLanguage lang = math::lang::MathLanguage::english_us);
    
    /**
     Lấy danh sách số thứ tự. Lấy trong list ảnh, nếu ngoai list anh. tự sinh data.

     @param from Số bắt đầu
     @param to Số kết thúc
     @return Danh sách số thứ tự
     */
    std::vector<math::resource::Image*> getOrdinals(int from, int to);
    
    /**
     Lấy danh sách toán tử (+, -, x, :, =, <, >)

     @param name Lọc tên
     @return Danh sách toán tử
     */
    std::vector<math::resource::Image*> getOperators();
    math::resource::Image* getOperator(std::string name="");
    /**
     Lấy danh sách tên người cho quiz. Tên được lưu vào json.

     @param number Số lượng tên cần lấy
     @return Danh sách tên
     */
    std::vector<math::resource::Image*> getPeopleNames(int number);
    
    /**
     Lấy danh sách các thứ trong tuần.
     
     @param date số thứ tự ngày cần lấy tên, =7, lấy tât cả.
     @return Danh sách thứ trong tuần
     */
    std::vector<math::resource::Image*> getDatesOfWeek();
    math::resource::Image* getDateOfWeek(int date);

    /**
     Lấy danh sách các tháng trong năm.
     
     @param date số thứ tự tháng cần lấy tên, =12, lấy tât cả.
     @return Danh sách tháng trong năm
     */
    std::vector<math::resource::Image*> getMonthsOfYear();
    math::resource::Image* getMonthOfYear(int month);

    /**
     Lấy danh sách các mùa trong năm.
     
     @param date số thứ tự mùa cần lấy tên, =4, lấy tât cả.
     @return Danh sách mùa trong năm
     */
    std::vector<math::resource::Image*> getSeasonsOfYear();
    math::resource::Image* getSeasonOfYear(int season);

    /**
     Lấy màu theo tên

     @param color tên màu
     @param white chấp nhận lấy màu trắng?
     @return Color4B
     */
    cocos2d::Color4B getColor4B(std::string color, bool white = false);
    
    /**
     Lấy màu theo tên.

     @param color Tên màu
     @param white Chấp nhận màu trắng?
     @return Color3B
     */
    cocos2d::Color3B getColor3B(std::string color, bool white = false);
    
    /**
     Lấy màu theo tên

     @param color Tên màu
     @param white Chấp nhận màu trắng
     @return Color4F
     */
    cocos2d::Color4F getColor4F(std::string color, bool white = false);

    
    /**
     Lấy danh sách icons trong file json

     @param json file json chứa icons
     @return Danh sách tất cả icons
     */
    std::vector<math::resource::Image*> getImageList(std::string json);
    
    /**
     Lấy danh sách icons từ file json

     @param json file json chưa icons
     @param number số icon cần lấy
     @param different điếm khác biệt các icons
     @param same Khóa lọc icons
     @param same_value Giá trị để lọc icon
     @return Danh sách icons
     */
    std::vector<math::resource::Image*> getImageList(std::string json, int number, std::string different, std::string same = "", cocos2d::Value same_value = cocos2d::Value::Null);
    
    /**
     Lấy danh sách icon từ danh sách cha

     @param imgs danh sách icons cha
     @param kind số icon cần lấy
     @param filter Điểm khác biệt các icons
     @param same Điểm chung các icons
     @param no_white Chấp nhận lấy màu trắng?
     @return Danh sách màu
     */
    std::vector<math::resource::Image*> getImageList(std::vector<math::resource::Image*> imgs, int number, std::string different, std::string same = "", cocos2d::Value same_value = cocos2d::Value::Null);
        
    /**
     Lấy danh sách icon theo list và số lượng

     @param json file json chưa icons
     @param img_list danh sách icons mẫu
     @param numbers Danh sách số lượng icon mỗi loại
     @param prop_key Thuộc tính mẫu
     @return Danh sách icons
     */
    std::vector<math::resource::Image*> getImageListNumber(std::string json, std::vector<math::resource::Image*> images, std::vector<int> numbers, std::string prop_key);
    
    /**
     Lấy danh sách icon theo list và số lượng

     @param imgs Danh sách icons cha
     @param images Danh sách icons mẫu
     @param numbers Danh sách số lượng
     @param prop_key Thuộc tính mẫu
     @return Danh sách icons
     */
    std::vector<math::resource::Image*> getImageListNumber(std::vector<math::resource::Image*> imgs, std::vector<math::resource::Image*> images, std::vector<int> numbers, std::string prop_key);
    

#pragma mark - Function support
    /**
     Lấy khóa so sánh theo tên

     @param key Tên khóa so sánh
     @return Khóa so sánh
     */
    math::comparision::Key getComparisionKey (std::string key);
    
    /**
     Lấy kích thước button answer

     @param size Kích thước
     @return Kích thước button cocos2d::Size
     */
    cocos2d::Size getAnswerSize(int size=1);

    void fixAnswerSize(float max_width = 0, float min_width = 100);
  
    /**
     Lấy danh sách index random

     @param number Số lượng index cần lấy
     @param size Kích thước dãy
     @return Danh sách index sau khi random
     */
    std::vector<int> randomArrayIndexes(int number, int size);
    
    /**
     Lấy danh sách điểm ngẫu nhiên trong khung

     @param item_size Kích thước các phần tử
     @param box_rect Kich thước vùng random
     @param number_item Số lượng phần tử
     @return Danh sách điểm
     */
    std::vector<cocos2d::Vec2> randomPositionsInRect(cocos2d::Size item_size, cocos2d::Rect box_rect, int number_item);
    
    void randomShow(std::vector<cocos2d::ui::Layout*> list, float delay = 0.0, math::animation::ObjectShowType show = math::animation::ScaleUp);
    void randomHide(std::vector<cocos2d::ui::Layout*> list, float delay = 0.0, math::animation::ObjectShowType revert_show = math::animation::ScaleUp);

    void randomShow(std::vector<Math_Object*> list, float delay = 0.0, math::animation::ObjectShowType show = math::animation::ScaleUp);
    void randomHide(std::vector<Math_Object*> list, float delay = 0.0, math::animation::ObjectShowType revert_show = math::animation::ScaleUp);

    void randomShow(std::vector<Math_Slot*> list, float delay = 0.0, math::animation::ObjectShowType show = math::animation::ScaleUp);
    void randomHide(std::vector<Math_Slot*> list, float delay = 0.0, math::animation::ObjectShowType revert_show = math::animation::ScaleUp);

    
#pragma mark - JSON Helper
    /**
     Random giá trị trong JSON member

     @param json json object cha
     @param member Tên member
     @return Giá trị kết quả
     */
    int randomRangeJson(rapidjson::Value& json, std::string member);
    
    /**
     Lấy size từ JSON object

     @param json JSON object cha
     @param member Tên thuộc tính lấy Size
     @return cocos2d::Size
     */
    cocos2d::Size getSizeJson(rapidjson::Value& json, std::string member);
    
    /**
     Lấy đường dẫn file ảnh theo theme

     @param path Đường dẫn file ảnh trong theme
     @return Đường dẫn file ảnh
     */
    std::string getUIPath(std::string path);
    
    /**
     Lấy JSON object

     @param parent JSON Object cha
     @param child_name Tên member
     @return JSON object con
     */
    rapidjson::Value& getJsonMember(rapidjson::Value& parent, std::string child_name);
    
    /**
     Lấy giá trị Bool của JSON

     @param parent JSON cha
     @param child_name Tên Member
     @param _default Giá trị mặc định
     @return Giá trị Bool
     */
    bool getJsonBool(rapidjson::Value& parent, std::string child_name, bool _default = true);
    
    /**
     Lấy Giá trị Int của JSON Member

     @param parent JSON cha
     @param child_name Tên Member
     @param _default Giá trị mặc định
     @return Giá trị Int
     */
    int getJsonInt(rapidjson::Value& parent, std::string child_name, int _default = 0);
    
    /**
     Lấy giá trị Float của JSON Member

     @param parent JSON cha
     @param child_name Tên Member
     @param _default Giá trị default
     @return Giá trị Float
     */
    float getJsonDouble(rapidjson::Value& parent, std::string child_name, float _default = 0);
    
    /**
     Lấy giá trị string của JSON Member

     @param parent JSON cha
     @param child_name Tên Member
     @param _default Giá trị default
     @return Giá trị String
     */
    std::string getJsonString(rapidjson::Value& parent, std::string child_name, std::string _default = "");
    
    
#pragma mark - Trial Manager
    /**
     Kiểm tra điều kiện dừng chơi thử khi kết thúc quiz.

     @return Dừng chơi thử?
     */
    bool checkStopTrialWhenComplete ();
    
    /**
     Kiểm tra điều kiện dừng chơi thử bên trong quiz

     @return Dừng chơi thử?
     */
    bool checkStopTrial ();

    bool stop_trial_when_completed = true;

private:
    cocos2d::ui::Layout*    _panel_disable;
    cocos2d::ui::ImageView* _bg_quiz = nullptr;
    cocos2d::ui::ImageView* _bg_panel;
    cocos2d::ui::ImageView* _bg_panel_sub;
    cocos2d::ui::ImageView* _color_selected;
    cocos2d::ui::Text*      _debug_text;
    cocos2d::Rect           right_bar_rect;
    cocos2d::Rect           bot_bar_rect;

    math::common::QuizType display_type = math::common::normal;
    
    //Audio
    bool isSpeaking=false;
    int voice_id = 0;
    void onPreloadSeqDone(std::string key);
    void onPlaySeqDone(std::string key);
    
    math::SequenceAudio* speak_question = nullptr;
    bool show_question = false;

    //
    std::vector<math::resource::Image*> _colors_img;
    std::vector<math::resource::Image*> _operators_img;

};
#endif /* Math_Quiz_h */
