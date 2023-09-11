#include <cstdio>
#include <iostream>
#include <thread>

#include "processbinder.h"
#include "utils.h"
#include "player_info.h"
#include "datareader.h"

HANDLE proc_handler;

uint16_t health_offset = 0x100;
uint16_t enemy_offset = 0xF4;
uint16_t location_offset = 0xA0;

uint8_t my_team;

std::vector<uintptr_t> enemy_loc_addr_list;
std::vector<location_info> player_coordinates_list;     // R&W by another thread
                                                        // R by main thread
//void gotoxy(int x, int y) {
//    COORD pos = {(short)x, (short)y};
//    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
//    SetConsoleCursorPosition(output, pos);
//}

void updateEnemyLocation(std::vector<location_info>* out){
    out->clear();
    for(auto e: enemy_loc_addr_list){
        std::vector<float> temp;
        readDataByInterval<float>(proc_handler, e + location_offset, 3, 0x4, &temp);
        location_info loc_info = {temp[0], temp[1], temp[2]};
        out->push_back(loc_info);
    }
}

void printEnemyLocation(std::vector<location_info>* out){
    for(int i = 0; i<out->size();i++){
        printf("Enemy %d : x=%f, y=%f, z=%f\n", i+1, out->at(i).x, out->at(i).y, out->at(i).z);
    }
}

void sub_task1(std::vector<location_info>* out){
    while(true){
        updateEnemyLocation(out);
//        printEnemyLocation(out);
//        gotoxy(0,0);
//        std::this_thread::sleep_for(std::chrono::microseconds(1000));
    }
}

int main(int args, char* argv[]){
    printf("=================Executing=================\n");
    ProcessBinder processManager("csgo.exe");

    // print client.dll base address and handler
    uintptr_t client_dll = processManager.get_client_dll();
    proc_handler = processManager.get_handler();
    logManager.log("client.dll base address: " + std::to_string(client_dll), logManager.INFO);

    // read player info list
    uintptr_t player_info_list_offset = 0x4DFFF7C;
    std::vector<uint32_t> players_info_list;
    readDataByInterval<uint32_t>(proc_handler, client_dll + player_info_list_offset, 10, 0x10, &players_info_list);

    // print player info
    
    for(int i = 0; i< players_info_list.size(); i++){
        printf("[INFO] Reading: %x\n", players_info_list[i]);

        // read team info
        if (i==0)
            my_team = readData<uint8_t>(proc_handler, players_info_list[i] + enemy_offset);
        else{
            uint8_t enemy_team = readData<uint8_t>(proc_handler, players_info_list[i] + enemy_offset);
            if (enemy_team != my_team){
                printf("[INFO] Enemy labeled!\n");
                enemy_loc_addr_list.push_back(players_info_list[i]);
            }
        }
    }

    // the sub-thread read enemy location into the vector
    std::thread th_readLocation(sub_task1, &player_coordinates_list);

    // main thread print the location of enemies
    th_readLocation.join();

    return 0;
}