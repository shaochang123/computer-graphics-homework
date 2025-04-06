#ifndef GRAPH_POLYGON_H_INCLUDED
#define GRAPH_POLYGON_H_INCLUDED
#ifndef GRAPH_INIT_H_INCLUDED
#include<graph/init.h>
#endif
#ifndef GRAPH_Line_H_INCLUED
#include<graph/Line.h>
#endif
struct ET{
    int ymax;
    double x;
    double delta;
    ET* next;
    
    ET() : next(nullptr) {}
    ET(int _ymax, double _x, double _delta) : ymax(_ymax), x(_x), delta(_delta), next(nullptr) {}
};

struct AET{
    int ymax;
    double x;
    double delta;
    AET* next;
    
    AET() : next(nullptr) {}
    AET(int _ymax, double _x, double _delta) : ymax(_ymax), x(_x), delta(_delta), next(nullptr) {}
};

// 边表数组，每个槽位存储一条扫描线的边表
ET* EdgeTable[maxn];

// 初始化边表
void initEdgeTable() {
    for (int i = 0; i < maxn; i++) {
        EdgeTable[i] = nullptr;
    }
}

// 插入边到ET表
void insertEdge(ET* &header, ET* edge) {
    if (!header) {
        header = edge;
        return;
    }
    
    // 按x值升序插入
    if (edge->x < header->x) {
        edge->next = header;
        header = edge;
        return;
    }
    
    ET* p = header;
    while (p->next && p->next->x < edge->x) {
        p = p->next;
    }
    
    edge->next = p->next;
    p->next = edge;
}

// 创建多边形的边表
void createEdgeTable(const std::vector<Point>& vertices) {
    initEdgeTable();
    
    int n = vertices.size();
    for (int i = 0; i < n; i++) {
        Point current = vertices[i];
        Point next = vertices[(i + 1) % n];
        
        // 如果是水平边，需要特殊处理
        if (current.y == next.y) {
            // 可以选择直接在这里填充水平线
            int y = current.y;
            int x1 = std::min(current.x, next.x);
            int x2 = std::max(current.x, next.x);
            for (int x = x1; x <= x2; x++) {
                if (x >= 0 && x < maxn && y >= 0 && y < maxn) {
                    setpixel(x,y, w);
                }
            }
            continue;
        }
        
        // 确保从y较小的点到y较大的点
        int ymin, ymax;
        double x;
        double delta;
        
        // 对于非水平边
        if (current.y < next.y) {
            ymin = current.y;
            ymax = next.y;
            x = current.x;
            delta = (double)(next.x - current.x) / (next.y - current.y);
        } else {
            ymin = next.y;
            ymax = current.y;
            x = next.x;
            delta = (double)(current.x - next.x) / (current.y - next.y);
        }
        
        // 确保边被正确插入，特别注意下边界
        ET* edge = new ET(ymax, x, delta);
        insertEdge(EdgeTable[ymin], edge);
    }
}

// 将ET中的边插入到AET中
void insertToAET(AET* &aet, ET* et) {
    while (et) {
        // 创建AET节点
        AET* edge = new AET(et->ymax, et->x, et->delta);
        
        // 插入到AET（按x值排序）
        if (!aet) {
            aet = edge;
        } else if (edge->x < aet->x) {
            edge->next = aet;
            aet = edge;
        } else {
            AET* p = aet;
            while (p->next && p->next->x < edge->x) {
                p = p->next;
            }
            edge->next = p->next;
            p->next = edge;
        }
        
        et = et->next;
    }
}

// 按x值对AET进行排序
void sortAET(AET* &aet) {
    if (!aet) return;
    
    AET* sorted = nullptr;
    while (aet) {
        AET* current = aet;
        aet = aet->next;
        
        if (!sorted || current->x < sorted->x) {
            current->next = sorted;
            sorted = current;
        } else {
            AET* p = sorted;
            while (p->next && p->next->x < current->x) {
                p = p->next;
            }
            current->next = p->next;
            p->next = current;
        }
    }
    
    aet = sorted;
}

// 修改填充扫描线函数
void fillScanLine(int y, AET* aet) {
    AET* p = aet;
    while (p && p->next) {
        // 使用floor和ceil确保捕获所有边界像素
        int x1 = static_cast<int>(floor(p->x));
        int x2 = static_cast<int>(ceil(p->next->x));
        
        // 包含端点
        for (int x = x1; x <= x2; x++) {
            if (x >= 0 && x < maxn && y >= 0 && y < maxn) {
                setpixel(x,y,w);
            }
        }
        p = p->next->next;
    }
}

// 更新AET中x值
void updateAET(AET* aet) {
    AET* p = aet;
    while (p) {
        p->x += p->delta;
        p = p->next;
    }
}

// 移除AET中的过期边
void removeExpiredEdges(AET* &aet, int y) {
    // 边在y+1过期，而不是y
    while (aet && aet->ymax < y+1) {
        AET* temp = aet;
        aet = aet->next;
        delete temp;
    }
    
    if (!aet) return;
    
    AET* p = aet;
    while (p->next) {
        if (p->next->ymax < y+1) {
            AET* temp = p->next;
            p->next = temp->next;
            delete temp;
        } else {
            p = p->next;
        }
    }
}

// 多边形扫描线填充
void fillPolygon(const std::vector<Point>& vertices) {
    // 找到多边形的y坐标范围
    int ymin = 2*maxn;
    int ymax = -1;
    for (const auto& point : vertices) {
        ymin = std::min(ymin, point.y);
        ymax = std::max(ymax, point.y);
    }
    
    // 创建边表
    createEdgeTable(vertices);
    
    // 活跃边表
    AET* aet = nullptr;
    
    // 扩展扫描范围确保覆盖所有点
    for (int y = ymin; y <= std::min(ymax+1, maxn-1); y++) {
        // 将新的边加入AET
        insertToAET(aet, EdgeTable[y]);
        
        // 移除过期的边
        removeExpiredEdges(aet, y);
        
        // 按x排序
        sortAET(aet);
        
        // 填充扫描线
        fillScanLine(y, aet);
        
        // 更新AET中的x值
        updateAET(aet);
    }
    
    // 清理内存
    AET* p = aet;
    while (p) {
        AET* temp = p;
        p = p->next;
        delete temp;
    }
}

// 添加点到多边形
void addPointToPolygon(std::vector<Point>& vertices, int x, int y) {
    vertices.push_back({x, y});
}

// 鼠标回调
std::vector<Point> polygonVertices;

// 绘制多边形
void drawPolygon(GLFWwindow *window) {
    double xpos, ypos;
    detectposition(window, xpos, ypos);
    if(polygonVertices.size()==1){
        drawLineBresenham(polygonVertices[0], {static_cast<int>(xpos), static_cast<int>(ypos)});
    }
    else if(polygonVertices.size()>=2){
        polygonVertices.push_back({static_cast<int>(xpos), static_cast<int>(ypos)});
        fillPolygon(polygonVertices);
        polygonVertices.pop_back();
    }
    render();//必须写上，不然切换后立马黑屏
}

void Polygon_Mouse_Pressed(GLFWwindow* window, int button, int action, int mods) {
    if (mode!=3) return;
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        detectposition(window, xpos, ypos);
        addPointToPolygon(polygonVertices, static_cast<int>(xpos), static_cast<int>(ypos));
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        // 右键闭合多边形
        if (polygonVertices.size() >= 2) {
            drawPolygon(window);//这里加上，和画直线一样，不然画完之后立马就没
            Cnt2Color.push_back(currentColor);
            cnt++;
            polygonVertices.clear();
        }
        else{
            std::cout << "Polygon must have at least 3 vertices." << std::endl;
        }
    }
}

// 键盘回调
void Polygon_Keyboard_Pressed(int key, int mods, int action) {
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        mode = 3; // 设置为多边形模式
        polygonVertices.clear();
        std::cout << "Polygon Mode" << std::endl;
    }
}

#endif
