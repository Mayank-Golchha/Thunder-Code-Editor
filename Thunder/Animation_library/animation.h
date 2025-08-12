//TODO : sort images by no

//to show different animation

#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <vector>
#include <gdiplus.h>
#include <filesystem>
#include <algorithm>

int match_no_(std::wstring file){
    //will find no in file name
    std::wstring n = L"";
    for (int i = 0;i<file.size();i++){
        if (file[i] >= L'0' && file[i] <= L'9') n += file[i];
    }
    return std::stoi(n);
}

class Animation{
    public:
        int index;
        int x_,y_;
        std::wstring folder_path;
        std::vector<Gdiplus::Image*> images;
        int isAnimationOver;
        int showAnimation;
        int lastFrame; //will save index of last animation frame

        Animation(std::wstring folder_path_){
            isAnimationOver = 1;
            showAnimation = 0;
            folder_path = folder_path_;
            x_ = 0;
            y_ = 0;
        }

        // ~Animation() {
            // clear();
        // }

        void clear(){
            for (auto img : images) {
                delete img;
            }
            images.clear();
        }

        void init(){
            //call whenever want to start animation
            index = 0;
            isAnimationOver = 0;
            showAnimation = 1;
            lastFrame = -1;
        }

        void load_images(){
            std::vector<std::wstring> paths;
            for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
                if (entry.is_regular_file()) {
                    std::wstring path = entry.path().wstring();
                    // images.push_back(new Gdiplus::Image(path.c_str()));
                    paths.push_back(path);
                }
            }
            //sorting needed to load correctly all frames
            std::sort(paths.begin(),paths.end(),[](const std::wstring file1,std::wstring file2){
                return match_no_(file1) < match_no_(file2);
            });

            for (int i = 0;i<paths.size();i++) images.push_back(new Gdiplus::Image(paths[i].c_str()));
        }

        void update(){
            lastFrame = index;
            index++;
            if (index >= images.size()) {
                isAnimationOver = 1;
                showAnimation = 0;
                index = 0;
            }
        }

        void show(HDC hdc,int x,int y){
            if (!isAnimationOver && showAnimation && images[index] != nullptr){
                Gdiplus::Graphics g(hdc);
                x_ = x; y_ = y;
                g.DrawImage(images[index],x,y);
            }
        }

        void loop_show(HDC hdc,int x,int y){
            Gdiplus::Graphics g(hdc);
            x_ = x; y_ = y;
            g.DrawImage(images[index],x,y);

        }

        void move_h(HDC hdc,int s_x,int s_y,int end_x,int step){
            // will move horizontally the animation from start to end in given step
            if (!isAnimationOver && showAnimation && images[index] != nullptr){
                Gdiplus::Graphics g(hdc);
                x_ = s_x; y_ = s_y;
                g.DrawImage(images[index],s_x,s_y);
            }
        }

};


class DinoAnimation{
    public:
        int x, y;
        std::wstring folder_path;
        std::vector<Gdiplus::Image*> images;
        int last_frame,index;
        int startX,startY,endX,endY;
        int x_offset,y_offset;

        DinoAnimation(std::wstring folder_path_,int x_,int y_,int x_offset_,int y_offset_){
            folder_path = folder_path_;
            x = x_; y = y_;
            x_offset = x_offset_; y_offset = y_offset_;
        }
        ~DinoAnimation() {
            clear();
        }

        void clear(){
            for (auto img : images) {
                delete img;
            }
            images.clear();
        }

        void init(int sx,int sy,int ex,int ey){
            //call whenever want to start animation
            index = 0;
            last_frame = -1;
            startX = sx; startY = sy;
            endX = ex; endY = ey;
        }

        void load_images(){
            std::vector<std::wstring> paths;
            for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
                if (entry.is_regular_file()) {
                    std::wstring path = entry.path().wstring();
                    // images.push_back(new Gdiplus::Image(path.c_str()));
                    paths.push_back(path);
                }
            }
            //sorting needed to load correctly all frames
            std::sort(paths.begin(),paths.end(),[](const std::wstring file1,std::wstring file2){
                return match_no_(file1) < match_no_(file2);
            });

            for (int i = 0;i<paths.size();i++) images.push_back(new Gdiplus::Image(paths[i].c_str()));
        }

        void update(){
            last_frame = index;
            index++;
            if (x > endX || x < 0){
               x_offset *= -1; 
               y_offset *= -1; 
            }
            x += x_offset; y += y_offset;

            if (index >= images.size()) {
                index = 0;
            }
        }

        void show(HDC hdc){
            Gdiplus::Graphics g(hdc);
            g.DrawImage(images[index],x,y);
        }


};

#endif