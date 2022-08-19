#ifndef BALL_H
#define BALL_H
#include <iostream>
#include <time.h>


#include "matrixStack.h"

class Ball
{
public:
    glm::vec3 move[50];
    glm::vec3 dir[50];
    glm::vec3 localVectorx;
    glm::vec3 localVectory;
    glm::vec3 localVectorz;
    int color[50] = {0};
    int num  = 50;
    Ball()
    {
        srand(time(NULL));
        float min = 1.0f,min_d = 2.0f;
        float max = 21.0f,max_d = 7.0f;
        float x, y, z;
        for (int i = 0; i < num; i++)
        {
            x = (max - min) * rand() / (RAND_MAX + 1.0) + min;
            y = (max - min) * rand() / (RAND_MAX + 1.0) + min;
            z = (max - min) * rand() / (RAND_MAX + 1.0) + min;
            move[i] = glm::vec3(x, y, z);
            x = (max_d - min_d) * rand() / (RAND_MAX + 1.0) + min_d;
            y = (max_d - min_d) * rand() / (RAND_MAX + 1.0) + min_d;
            z = (max_d - min_d) * rand() / (RAND_MAX + 1.0) + min_d;
            dir[i] = glm::vec3(x, y, z);
            // dir[i] = glm::vec3(5.0f, 5.0f, 5.0f);
            color[i] = 0;
        }
        move[0] = glm::vec3(10.0,10.0f,20.0f);
        move[1] = glm::vec3(20.0,10.0f,20.0f);
        dir[0] = glm::vec3(3.0f,0.0f,0.0f);
        dir[1] = glm::vec3(-3.0f,0.0f,0.0f);

    }
    ~Ball() {}
    void ball_collision(float radians, float deltaTime, glm::vec3 cubePos)
    {
        for(int j = 0; j < num; j++){

            
            // MatrixStack loc_rotate;
            // loc_rotate.Save(glm::rotate(loc_rotate.Top(), glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

            // collision test
            // floor wall cube test (cannot move)
            for (int i = 0; i < 3; i++)
            {
                if (move[j][i] - radians < 0 || move[j][i] + radians > 21)
                { // || (movex - radians < 13.5 && movey >= 0 && movey <= 6 && movez >= 7.5 && movez <= 13.5))
                    dir[j][i] *= (-1);
                    color[j] = 1;
                    // std::cout <<"~1" <<std::endl;
                }
            }

            // cube (move)
            glm::vec3 local;
            std::cout << "localvectorx" << localVectorx[0] <<" " << localVectorx[1] <<" " << localVectorx[2] <<std::endl;
            std::cout << "localvectory" << localVectory[0] <<" " << localVectory[1] <<" " << localVectory[2] <<std::endl;
            std::cout << "localvectorz" << localVectorz[0] <<" " << localVectorz[1] <<" " << localVectorz[2] <<std::endl;
            local[0] = (move[j][0] - cubePos[0]) * localVectorx[0] + (move[j][1] - cubePos[1]) * localVectorx[1] + (move[j][2] - cubePos[2]) * localVectorx[2];
            local[1] = (move[j][0] - cubePos[0]) * localVectory[0] + (move[j][1] - cubePos[1]) * localVectory[1] + (move[j][2] - cubePos[2]) * localVectory[2];
            local[2] = (move[j][0] - cubePos[0]) * localVectorz[0] + (move[j][1] - cubePos[1]) * localVectorz[1] + (move[j][2] - cubePos[2]) * localVectorz[2];
            // std::cout << "cube : " << cubePos[0] << " " << cubePos[1] << " " << cubePos[2] << std::endl;
            // std::cout << "0 move : " << move[0] << " " << move[1] << " " << move[2] << std::endl;
            // std::cout << "dir : " << dir[0] << std::endl;
            // std::cout << "local : " << local[0] << " " << local[1] << " " << local[2] << std::endl;
            if (local[0] + radians > 0 && local[0] - radians < 6 && local[1] >= 0 && local[1] <= 6 && local[2] >= 0 && local[2] <= 6)
            {
                dir[j][0] *= (-1);
                color[j] = 1;
                // std::cout <<"~2" <<std::endl;
                // dir = glm::vec3(loc_rotate.Top()*glm::vec4(dir,0.0f));
            }
            if (local[1] + radians > 0 && local[1] - radians < 6 && local[0] >= 0 && local[0] <= 6 && local[2] >= 0 && local[2] <= 6)
            {
                dir[j][1] *= (-1);
                color[j] = 1;
                // std::cout <<"~3" <<std::endl;
                // dir = glm::vec3(loc_rotate.Top()*glm::vec4(dir,0.0f));
            }
            if (local[2] + radians > 0 && local[2] - radians < 6 && local[0] >= 0 && local[0] <= 6 && local[1] >= 0 && local[1] <= 6)
            {
                dir[j][2] *= (-1);
                color[j] = 1;
                // std::cout <<"~4" <<std::endl;
                // dir = glm::vec3(loc_rotate.Top()*glm::vec4(dir,0.0f));
            }

            // ball move  //speed up          
            for(int t = 0; t < num; t++){
                float lenght;
                for(int q = t+1; q < num;q++){
                    lenght = sqrt(pow(move[t][0] - move[q][0],2) + pow(move[t][1] - move[q][1],2) + pow(move[t][2] - move[q][2],2));
                    if(lenght < radians*2){
                        for(int k = 0; k < 3; k++){
                            dir[t][k]*=(-1);
                            dir[q][k]*=(-1);
                        }
                        color[t] = 2;
                        color[q] = 2;
                    }
                }
            }

            //  move forward or backward
            for (int i = 0; i < 3; i++)
            {
                move[j][i] = move[j][i] + (deltaTime * dir[j][i]);
            }
        }
        return;
    }
    void comput_axis(glm::vec3 cubePos, float cube_scale)
    {
        float lenght = 0;
        // cube ball x
        MatrixStack localMatrix;
        // localMatrix.Save(glm::translate(localMatrix.Top(), cubePos));
        localMatrix.Save(glm::rotate(localMatrix.Top(), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
        localMatrix.Push();
        localMatrix.Save(glm::translate(localMatrix.Top(), glm::vec3(1, 0, 0)));
        localVectorx = glm::vec3(0.0f, 0.0f, 0.0f);
        localVectorx = localMatrix.Top() * glm::vec4(localVectorx, 1.0f);
        // localVectorx = localVectorx - cubePos;
        std::cout<<localVectorx[0] << " "<<localVectorx[1] << " "<<localVectorx[2]<<std::endl;
        // normalize
        lenght = sqrt(pow(localVectorx[0], 2) + pow(localVectorx[1], 2) + pow(localVectorx[2], 2));
        localVectorx /= lenght;
        std::cout<<localVectorx[0] << " "<<localVectorx[1] << " "<<localVectorx[2]<<std::endl;
        localMatrix.Pop();

        // cube ball------y
        localMatrix.Push();
        localMatrix.Save(glm::translate(localMatrix.Top(), glm::vec3(0, cube_scale, 0)));
        localVectory = glm::vec3(0.0f, 0.0f, 0.0f);
        localVectory = localMatrix.Top() * glm::vec4(localVectory, 1.0f);
        // normalize
        lenght = sqrt(pow(localVectory[0], 2) + pow(localVectory[1], 2) + pow(localVectory[2], 2));
        localVectory /= lenght;
        localMatrix.Pop();

        // cube ball------z
        localMatrix.Push();
        localMatrix.Save(glm::translate(localMatrix.Top(), glm::vec3(0, 0, cube_scale)));
        localVectorz = glm::vec3(0.0f, 0.0f, 0.0f);
        localVectorz = localMatrix.Top() * glm::vec4(localVectorz, 1.0f);
        // normalize
        lenght = sqrt(pow(localVectorz[0], 2) + pow(localVectorz[1], 2) + pow(localVectorz[2], 2));
        localVectorz /= lenght;
        localMatrix.Pop();
    }
};

#endif