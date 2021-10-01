#define _CRT_SECURE_NO_WARNINGS
#include "inoutput.h"
#include "commands.h"


using namespace std;



int main()
{
    InputResult Res;
    Res = FullInputFunc();
    int PicsCount = Res.PicsCount;
    QueueElement* ptr_Queue = Res.ptr_QueueArr;
    Pic* ptr_Pics = Res.ptr_Pics;

    for (int i = 0; i < Res.CommCount; i++) {
        if (strcmp(ptr_Queue[i].CommName, "rotate") == 0) {
            for (int j = 0; j < PicsCount; j++) {
                rotate(&ptr_Pics[j], ptr_Queue[i].CommArgue);
            }
        }
        else if (strcmp(ptr_Queue[i].CommName, "bright") == 0) {
            if (ptr_Queue[i].CommPics[0] == 0) {
                for (int j = 0; j < PicsCount; j++) {
                    bright(&ptr_Pics[j], ptr_Queue[i].CommArgue);
                }
            }
            else {
                for (int j = 0; j < ptr_Queue[i].CommPicsCount; j++) {
                    bright(&ptr_Pics[ptr_Queue[i].CommPics[j] - 1], ptr_Queue[i].CommArgue);
                }
            }
        }
        else if (strcmp(ptr_Queue[i].CommName, "flip") == 0) {
            if (ptr_Queue[i].CommPics[0] == 0) {
                for (int j = 0; j < PicsCount; j++) {
                    flip(&ptr_Pics[j], ptr_Queue[i].CommArgue);
                }
            }
            else {
                for (int j = 0; j < ptr_Queue[i].CommPicsCount; j++) {
                    flip(&ptr_Pics[ptr_Queue[i].CommPics[j] - 1], ptr_Queue[i].CommArgue);
                }
            }
        }
        else if (strcmp(ptr_Queue[i].CommName, "downscale") == 0) {
            for (int j = 0; j < PicsCount; j++) {
                downscale(&ptr_Pics[j], ptr_Queue[i].CommArgue);
            }
        }
        else if (strcmp(ptr_Queue[i].CommName, "upscale") == 0) {
            for (int j = 0; j < PicsCount; j++) {
                upscale(&ptr_Pics[j], ptr_Queue[i].CommArgue);
            }
        }
        else if (strcmp(ptr_Queue[i].CommName, "crop") == 0) {
            for (int j = 0; j < PicsCount; j++) {
                crop(&ptr_Pics[j], ptr_Queue[i].CommArgue);
            }
        }
    }
    OutputFunc(PicsCount, ptr_Pics);
    return 0;
}


