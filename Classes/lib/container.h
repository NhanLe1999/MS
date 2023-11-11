//
// Created by anhquan on 01/04/21.
//

#ifndef BUILD_ANDROID_CONTAINER_H
#define BUILD_ANDROID_CONTAINER_H
/*
 * Việc gọi hàm qua lại giữa C và Java có một số hạn chế sau:
 * * Các hàm callback trong C++ không thể (khó để) truyền làm tham số để gọi từ một hàm trong Java
 * *  và ngược lại.
 * * Một số kiểu dữ liệu không thể  (khó) dùng làm tham số truyển qua lại giữa hai hàm C++ và Java
 * *  cùng dùng chung giá trị của tham số đó.
 * Nên file này sẽ lưu các biến dùng hỗ trợ việc gọi hàm java trong C và C trong Java
 * Ví dụ sử dụng:
 * * readDatabase(các_tham_số, t_callback)  {
 *      gọi_1_hàm_tương_tự_trong_Java("class", "tên_method", các_tham_số);
 * *}
 * * Do không thể (Khó) truyền t_callback làm tham số cho method tương tự trong java, nên ở hàm callback
 * * trong java ta sẽ gọi hàm  native, hàm native gọi một hàm C++ thực hiện t_callback
 * File này chứa các hàm lưu lại các t_callback để phía java có thể gọi lại.
 * Nói chung, file này lưu hàm callback cần gọi lại để thực hiện xong phía java thì gọi.
 */
#include "MMFirebaseDatabase.h"
#include <functional>

std::function<void()> _voidCallBack;

#endif //BUILD_ANDROID_CONTAINER_H
