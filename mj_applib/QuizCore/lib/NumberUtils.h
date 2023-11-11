//
//  NumberUtils.h
//  MJStory
//
//  Created by HuyPhuong on 3/27/18.
//

#ifndef NumberUtils_h
#define NumberUtils_h

#include <stdio.h>
#include "cocos2d.h"
#include "Math_Libs.h"

namespace math {
    
    namespace number {
        
        static inline std::string getNumberText(int number, math::lang::MathLanguage lang = math::lang::MathLanguage::english_us) {
				std::string ones[] = { "zero", "one","two","three","four","five","six","seven","eight","nine", "ten" };
				std::string teen[] = { "eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen" };
				std::string tens[] = { "twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety" };
			if (lang == math::lang::MathLanguage::vietnamese) {
				std::string ones_vi[] = { "không", "một","hai","ba","bốn","năm","sáu","bảy","tám","chín", "mười" };
				std::string teen_vi[] = { "eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen" };
				std::string tens_vi[] = { "twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety" };
				*ones = *ones_vi;
				*teen = *teen_vi;
				*tens = *tens_vi;
			}
			
            
            if (number < 11) {
                return ones[number];
            } else if (number < 20) {
                return teen[number-11];
            } else if (number < 100) {
                std::string result = "";
                int _tens = number / 10;
                int _ones = number % 10;
                
                result = tens[_tens-2];
                if (_ones == 0) {
                    return result;
                } else {
                    result = result + "-" + ones[_ones];
                    return result;
                }
            } else {
                return "one hundred";
            }
        }
        
        static inline std::vector<std::string> getNumberAudio(int number, math::lang::MathLanguage lang = math::lang::MathLanguage::english_us) {
            std::string ones[] = {"0", "1","2","3","4","5","6","7","8","9", "10"};
            std::string teen[] = {"11","12","13","14","15","16","17","18","19"};
            std::string tens[] = {"20","30","40","50","60","70","80","90"};
            
            std::vector<std::string> audios;
            if (number < 11) {
                audios.push_back(ones[number]);
            } else if (number < 20) {
                audios.push_back(teen[number-11]);
            } else if (number < 100) {
                int _tens = number / 10;
                int _ones = number % 10;
                
                audios.push_back(tens[_tens-2]);
                if (_ones != 0) {
                    audios.push_back(ones[_ones]);
                }
            } else {
                audios.push_back("1");
                audios.push_back("100");
            }
            
            std::vector<std::string> audios_fixed;
            for (std::string txt : audios) {
				if (lang == math::lang::MathLanguage::english_us) {
					audios_fixed.push_back(cocos2d::StringUtils::format("numbers/%s.mp3", txt.c_str()));
				}
				else {
					audios_fixed.push_back(cocos2d::StringUtils::format("numbers/%s_vi.mp3", txt.c_str()));
				}
                
            }
            return audios_fixed;
        }
        
        static inline math::resource::Image* getNumber(int number, math::lang::MathLanguage lang = math::lang::MathLanguage::english_us) {
            auto image = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%sgameui/number/%d.png", m_theme->getThemePath().c_str(), number));
            
            //name 1
            math::resource::Voice voice;
            voice.voice_id = 1;
            voice.paths = math::number::getNumberAudio(number,lang);
            math::resource::Text text;
            text.text = cocos2d::StringUtils::toString(number);
            text.voices.push_back(voice);
            math::resource::Property prop_number;
            prop_number.type = math::resource::Property::PROP;
            prop_number.key = "name_1";
            prop_number.value = text;
            
            //name n
            math::resource::Text text2;
            text2.text = math::number::getNumberText(number,lang);
            text2.voices.push_back(voice);
            math::resource::Property prop_text;
            prop_text.type = math::resource::Property::PROP;
            prop_text.key = "name_n";
            prop_text.value = text2;
            
            math::resource::PropertiesSet prop_set;
            prop_set.lang = m_lang->getLanguage();
            prop_set.properties.push_back(prop_number);
            prop_set.properties.push_back(prop_text);
            image->properties.push_back(prop_set);
            
            math::resource::Property prop_cmp;
            prop_cmp.type = math::resource::Property::COMP;
            prop_cmp.key = "number";
            prop_cmp.compare_value = number;
            image->comparisions.push_back(prop_cmp);
            
            return image;
        }
    }
    
    namespace character {
        static inline math::resource::Image* getCharacter(int index, bool lower) {
            std::string texts[] = {"a", "b","c","d","e","f","g","h","i","j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
            
            if (index < 0)
                index = 0;
            
            if (index>25)
                index = index % 26;
            
            auto image = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%sgameui/character/%s_%d.png", m_theme->getThemePath().c_str(), texts[index].c_str(), lower?0:1));
            
            //name 1
            math::resource::Voice voice;
            voice.voice_id = 1;
            voice.paths.push_back("");
            math::resource::Text text;
            text.text = texts[index];
            text.voices.push_back(voice);
            math::resource::Property prop_number;
            prop_number.type = math::resource::Property::PROP;
            prop_number.key = "name_1";
            prop_number.value = text;
            
            math::resource::PropertiesSet prop_set;
            prop_set.lang = m_lang->getLanguage();
            prop_set.properties.push_back(prop_number);
            image->properties.push_back(prop_set);
            
            math::resource::Property prop_cmp;
            prop_cmp.type = math::resource::Property::COMP;
            prop_cmp.key = "number";
            prop_cmp.compare_value = index;
            image->comparisions.push_back(prop_cmp);
            
            return image;
        }
    }
    
    namespace ordinal {
        static inline std::string getOrdinalText(int number) {
            std::string ones[] = {"zeroth", "first","second","third","fourth","fifth","sixth","seventh","eight","nine", "ten"};
            std::string teen[] = {"eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen"};
            std::string tens[] = {"twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"};
            
            if (number < 11) {
                return ones[number];
            } else if (number < 20) {
                return teen[number-11];
            } else if (number < 100) {
                std::string result = "";
                int _tens = number / 10;
                int _ones = number % 10;
                
                result = tens[_tens-2];
                if (_ones == 0) {
                    return result;
                } else {
                    result = result + "-" + ones[_ones];
                    return result;
                }
            } else {
                return "one hundred";
            }
        }
        
        static inline std::string getOrdinalNumber(int number) {
            std::string ones[] = {"0th", "1st","2nd","3rd","4th","5th","6th","7th","8th","9th", "10th"};
            std::string teen[] = {"11th","12th","13th","14th","15th","16th","17th","18th","19th"};
            std::string tens[] = {"20th","30th","40th","50th","60th","70th","80th","90th"};
            
            if (number < 11) {
                return ones[number];
            } else if (number < 20) {
                return teen[number-11];
            } else if (number < 100) {
                std::string result = "";
                int _tens = number / 10;
                int _ones = number % 10;
                
                result = tens[_tens-2];
                if (_ones == 0) {
                    return result;
                } else {
                    result = result + "-" + ones[_ones];
                    return result;
                }
            } else {
                return "100th";
            }
        }
        
        static inline std::vector<std::string> getOrdinalAudio(int number) {
            std::string ones[] = {"0", "1","2","3","4","5","6","7","8","9", "10"};
            std::string teen[] = {"11","12","13","14","15","16","17","18","19"};
            std::string tens[] = {"20","30","40","50","60","70","80","90"};
            
            std::vector<std::string> audios;
            if (number < 11) {
                audios.push_back(ones[number]);
            } else if (number < 20) {
                audios.push_back(teen[number-11]);
            } else if (number < 100) {
                int _tens = number / 10;
                int _ones = number % 10;
                
                audios.push_back(tens[_tens-2]);
                if (_ones != 0) {
                    audios.push_back(ones[_ones]);
                }
            } else {
                audios.push_back("1");
                audios.push_back("100");
            }
            
            std::vector<std::string> audios_fixed;
            for (std::string txt : audios) {
                audios_fixed.push_back(cocos2d::StringUtils::format("ordinals/%s.mp3", txt.c_str()));
            }
            return audios_fixed;
        }
        
        static inline math::resource::Image* getOrdinal(int number) {
            auto image = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%sgameui/number/%d.png", m_theme->getThemePath().c_str(), number));
            
            //name 1
            math::resource::Voice voice;
            voice.voice_id = 1;
            voice.paths = math::ordinal::getOrdinalAudio(number);
            
            math::resource::Text text;
            text.text = math::ordinal::getOrdinalNumber(number);
            text.voices.push_back(voice);
            math::resource::Property prop_number;
            prop_number.type = math::resource::Property::PROP;
            prop_number.key = "name_1";
            prop_number.value = text;
            
            //name n
            math::resource::Text text2;
            text2.text = math::ordinal::getOrdinalText(number);
            text2.voices.push_back(voice);
            math::resource::Property prop_text;
            prop_text.type = math::resource::Property::PROP;
            prop_text.key = "name_n";
            prop_text.value = text2;
            
            math::resource::PropertiesSet prop_set;
            prop_set.lang = m_lang->getLanguage();
            prop_set.properties.push_back(prop_number);
            prop_set.properties.push_back(prop_text);
            image->properties.push_back(prop_set);
            
            math::resource::Property prop_cmp;
            prop_cmp.type = math::resource::Property::COMP;
            prop_cmp.key = "number";
            prop_cmp.compare_value = number;
            image->comparisions.push_back(prop_cmp);
            
            return image;
        }
    }
}
#endif /* NumberUtils_h */
