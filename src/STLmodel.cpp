#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <string.h>
#include <GLFW/glfw3.h>
#include <iomanip>
#include "STLmodel.h"
// https://blog.csdn.net/u013378269/article/details/105989538
STLmodel::STLmodel()
{
}
STLmodel::~STLmodel()
{
}
bool STLmodel::LoadSTLfile(const char *stlFileName)
{
    FILE *file = NULL;
    errno_t err;                    // record error
    triangleNum = 0;                // record triangle's num
    file = fopen(stlFileName, "r"); // err = 0 success,err != 0 fail
    if (file == NULL)
    {
        std::cout << "Failed to open stlfile" << std::endl;
        return false;
    }

    char buffer[133];
    char str1[80];
    char str2[80];
    int lineno = 0; // record line number
    Vertex tri1;
    Vertex tri2;
    Vertex tri3;

    // read file
    fgets(buffer, 133, file);
    lineno++;
    sscanf(buffer, "%s %s", str1, str2); // solid filename.stl
    // std::cout << str1 << " " << str2 << std::endl;
    // assert(_strcmpi(str1, "solid") == 0);
    if (strcmp(str1, "solid") != 0)
    {
        std::cout << "Failed to read solid filename.stl" << std::endl;
        return false;
    }

    m_MeshTri.clear();
    while (1)
    {

        // read triangle
        lineno++;
        fgets(buffer, 133, file);
        sscanf(buffer, "%s %s %f %f %f", str1, str2, &(tri1.normal.xn), &(tri1.normal.yn), &(tri1.normal.zn)); // facet normal x y z
        if (strcmp(str1, "endsolid") == 0)
        {
            std::cout << "end" << std::endl;
            break;
        }
        //  std::cout <<"b" << buffer << std::endl;
        //  std::cout << str1<< " " << str2 << tri1.normal.xn << " " << tri1.normal.yn << " " << tri1.normal.zn <<std::endl;
        // std::cout << tri1.normal.xn << " " << tri1.normal.yn << " " << tri1.normal.zn << std::endl;
        if (strcmp(str1, "facet") != 0 || strcmpi(str2, "normal") != 0)
        {
            std::cout << "Failed to read facet normal x y z" << std::endl;
            continue;
        }
        else
        {
            tri3.normal.xn = tri2.normal.xn = tri1.normal.xn;
            tri3.normal.yn = tri2.normal.yn = tri1.normal.yn;
            tri3.normal.zn = tri2.normal.zn = tri1.normal.zn;
        }

        fgets(buffer, 133, file);
        lineno++;
        sscanf(buffer, " %*s %*s"); // outer loop

        fgets(buffer, 133, file);
        lineno++;
        sscanf(buffer, "%s %f %f %f", str1, &(tri1.position.x), &(tri1.position.y), &(tri1.position.z)); // Vector x y z
        // std::cout << "b1 " << buffer << std::endl;
        // std::cout << str1 << " " << tri1.position.x << " " << tri1.position.y << " " << tri1.position.z << std::endl;
        fgets(buffer, 133, file);
        lineno++;
        sscanf(buffer, "%s %f %f %f", str1, &(tri2.position.x), &(tri2.position.y), &(tri2.position.z)); // Vector x y z
        // std::cout << "b2 " << buffer << std::endl;
        // std::cout << str1 << " " << tri2.position.x << " " << tri2.position.y << " " << tri2.position.z << std::endl;
        fgets(buffer, 133, file);
        lineno++;
        sscanf(buffer, "%s %f %f %f", str1, &(tri3.position.x), &(tri3.position.y), &(tri3.position.z)); // Vector x y z
        // std::cout << "b3 " << buffer << std::endl;
        // std::cout << str1 << " " << tri3.position.x << " " << tri3.position.y << " " << tri3.position.z << std::endl;
        fgets(buffer, 133, file);
        lineno++;
        sscanf(buffer, " %*s"); // endloop

        fgets(buffer, 133, file);
        lineno++;
        sscanf(buffer, " %*s"); // endfacet

        // record max min that can easy to scale、translate、rotate
        {
            if (triangleNum == 0)
            {
                m_MaxPos[0] = m_MinPos[0] = tri1.position.x;
                m_MaxPos[1] = m_MinPos[1] = tri1.position.y;
                m_MaxPos[2] = m_MinPos[2] = tri1.position.z;
            }
            triangleNum += 3;

            // maximum
            if (m_MaxPos[0] < tri1.position.x)
                m_MaxPos[0] = tri1.position.x;
            if (m_MaxPos[0] < tri2.position.x)
                m_MaxPos[0] = tri2.position.x;
            if (m_MaxPos[0] < tri3.position.x)
                m_MaxPos[0] = tri3.position.x;
            if (m_MaxPos[1] < tri1.position.y)
                m_MaxPos[1] = tri1.position.y;
            if (m_MaxPos[1] < tri2.position.y)
                m_MaxPos[1] = tri2.position.y;
            if (m_MaxPos[1] < tri3.position.y)
                m_MaxPos[1] = tri3.position.y;
            if (m_MaxPos[2] < tri1.position.z)
                m_MaxPos[2] = tri1.position.z;
            if (m_MaxPos[2] < tri2.position.z)
                m_MaxPos[2] = tri2.position.z;
            if (m_MaxPos[2] < tri3.position.z)
                m_MaxPos[2] = tri3.position.z;
            // minimum
            if (m_MinPos[0] > tri1.position.x)
                m_MinPos[0] = tri1.position.x;
            if (m_MinPos[0] > tri2.position.x)
                m_MinPos[0] = tri2.position.x;
            if (m_MinPos[0] > tri3.position.x)
                m_MinPos[0] = tri3.position.x;
            if (m_MinPos[1] > tri1.position.y)
                m_MinPos[1] = tri1.position.y;
            if (m_MinPos[1] > tri2.position.y)
                m_MinPos[1] = tri2.position.y;
            if (m_MinPos[1] > tri3.position.y)
                m_MinPos[1] = tri3.position.y;
            if (m_MinPos[2] > tri1.position.z)
                m_MinPos[2] = tri1.position.z;
            if (m_MinPos[2] > tri2.position.z)
                m_MinPos[2] = tri2.position.z;
            if (m_MinPos[2] > tri3.position.z)
                m_MinPos[2] = tri3.position.z;
        }
        m_MeshTri.push_back(tri1);
        m_MeshTri.push_back(tri2);
        m_MeshTri.push_back(tri3);
        // std::cout << "1 " << tri1.position.x << " " << tri1.position.y << " " << tri1.position.z << std::endl;
        // std::cout << "2 " << tri2.position.x << " " << tri2.position.y << " " << tri2.position.z << std::endl;
        // std::cout << "3 " << tri3.position.x << " " << tri3.position.y << " " << tri3.position.z << std::endl;
    }
    fclose(file);

    m_scale = m_scale < (1.0 / (m_MaxPos[0] - m_MinPos[0])) ? m_scale : (1.0 / (m_MaxPos[0] - m_MinPos[0]));
    m_scale = m_scale < (1.0 / (m_MaxPos[1] - m_MinPos[1])) ? m_scale : (1.0 / (m_MaxPos[1] - m_MinPos[1]));
    m_scale = m_scale < (1.0 / (m_MaxPos[2] - m_MinPos[2])) ? m_scale : (1.0 / (m_MaxPos[2] - m_MinPos[2]));
    std::cout <<triangleNum << std::endl;
    return true;
}
