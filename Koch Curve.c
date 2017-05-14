#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "newgraph.h"

#define DEFAULT_ORDER 3
#define SIZE_FACTOR 0.1
#define TIMER_ID 1
#define TIME_INTERVAL 1000

struct Point {
    double x;
    double y;
};

typedef enum {
    ZOOM_IN,
    ZOOM_OUT
} zoom_type;

void TimerEventProcess(int timerID);
void MouseEventProcess(int x, int y, int button, int event);

void GetInfo(void);
void DrawCurve(int *order_in_use, int order);
void DrawForward(int *order_in_use, int order);
void InitCurvePosition(int *order_in_use, int order);
void RefreshDisplay(void);
void rotate(void);
void zoom(void);

static bool isRightMouseDown;
static ZoomType;
static struct Point CurrentPoint, PreviousPoint, CenterPoint;
static int Order;
static int *order_in_use;
static int EvolutionCount;
static int *EvolutionCount_in_use;
static double EdgeLength = 3.00;
struct curveADT *curve;
static double angle = 0;
extern double CurrentAngle;


void Main()
{
    SetWindowTitle("Koch Curve Program");
    InitGraphics();
    CenterPoint.x = GetWindowWidth() / 2;
    CenterPoint.y = GetWindowHeight() / 2;
    GetInfo();
    
    //DrawCurve();
    registerMouseEvent(MouseEventProcess);
	registerTimerEvent(TimerEventProcess);
    startTimer(TIMER_ID, TIME_INTERVAL);
}

void GetInfo(void)
{
    //char temp;

    order_in_use = GetBlock(sizeof(int));
    EvolutionCount_in_use = GetBlock(sizeof(int));

    InitConsole();
    printf("Input the order of Koch Curve:\n");
    scanf("%d", &Order);
    getchar();
    //*order_in_use = Order;
    printf("Input the edge length of the original curve:\n");
    scanf("%lf", &EdgeLength);
    *EvolutionCount_in_use = EvolutionCount = 1;
    //DrawCurve(order_in_use, Order);
    //printf("length:%f\n", EdgeLength);
    //printf("Let's draw!\n");
    //FreeConsole();
}

void DrawCurve(int *order_in_use, int order)
{
    InitCurvePosition(order_in_use, order);
    //printf("TEST:%d, %d, %f\n", order, *order_in_use, EdgeLength);
    DrawForward(order_in_use, order);
    turn(-2*pi/3);
    *order_in_use = order;
    DrawForward(order_in_use, order);
    turn(-2*pi/3);
    *order_in_use = order;
    DrawForward(order_in_use, order);
}

void InitCurvePosition(int *order_in_use, int order)
{
    double x0;// = GetWindowWidth() / 2 - EdgeLength / 2;
    double y0;// = GetWindowHeight() / 2 - EdgeLength / 2 / 1.732; //sqrt(3)
    double radius;

    //printf("centerpoint: %f, %f\n", CenterPoint.x, CenterPoint.y);
    radius = EdgeLength / 1.732; //sqrt(3)
    x0 = CenterPoint.x - radius * cos(angle + pi/6);
    y0 = CenterPoint.y - radius * sin(angle + pi/6);
    CurrentAngle = angle + pi/3;
    *order_in_use = order;
    MovePen(x0, y0);
    //printf("%f, %f; %f, %f; %f, %f\n", CenterPoint.x, CenterPoint.y, x0, y0, CurrentAngle, angle);
}

void DrawForward(int *order_in_use, int order)
{
    int temp;

    if (*order_in_use == 0) {
        forward(EdgeLength / pow(3, order));
        //printf("finish\n");
    } else {
        //printf("here\n");
        (*order_in_use)--;
        temp = *order_in_use;
        DrawForward(order_in_use, order);
        turn(pi/3);
        *order_in_use = temp;
        DrawForward(order_in_use, order);
        turn(-2*pi/3);
        *order_in_use = temp;
        DrawForward(order_in_use, order);
        turn(pi/3);
        *order_in_use = temp;
        DrawForward(order_in_use, order);
    }
}

void MouseEventProcess(int x, int y, int button, int event)
{
    //begin:get the trace of the mouse.
    PreviousPoint.x = CurrentPoint.x;
    PreviousPoint.y = CurrentPoint.y;
    CurrentPoint.x = ScaleXInches(x);
    CurrentPoint.y = GetWindowHeight() - ScaleXInches(y);
    //end:get the trace of the mouse.

    switch (event) {
        case BUTTON_DOWN:
            switch (button) {
                case LEFT_BUTTON:
                    //DrawCurve();
                    //MovePen(0, 0);
                    //DrawLine(GetWindowWidth()/2, GetWindowHeight()/2);
                    break;
                case RIGHT_BUTTON:
                    //ClearWindow();
                    //RefreshDisplay();
                    isRightMouseDown = 1;
                    break;
            }
            break;
        case BUTTON_UP:
            isRightMouseDown = 0;
            break;
        case MOUSEMOVE:
            if (isRightMouseDown) {
                //printf("here\n");
                rotate();
            }
            break;
        case ROLL_UP:
            ZoomType = ZOOM_OUT;
            zoom();
            break;
        case ROLL_DOWN:
            ZoomType = ZOOM_IN;
            zoom();
            break;
    }
}

void TimerEventProcess(int timerID)
{
    switch (timerID) {
        case TIMER_ID:
            printf("here\n");
            ClearWindow();
            DrawCurve(EvolutionCount_in_use, EvolutionCount);
            if (EvolutionCount++ < Order);
            else cancelTimer(TIMER_ID);
            break;
    }
}

void RefreshDisplay(void)
{
    //printf("here\n");
    SetEraseMode(TRUE);
    StartFilledRegion(1);
    DrawRectangle(0, 0, GetWindowWidth(), GetWindowHeight()); 
    EndFilledRegion();
    MovePen(CurrentPoint.x, CurrentPoint.y);
    SetEraseMode(FALSE);
}

void DrawRectangle(double x, double y, double width, double height)
{
    MovePen(x, y);
    DrawLine(width, 0);
    DrawLine(0, height);
    DrawLine(-width, 0);
    DrawLine(0, -height);
}

void rotate(void)
{
    double x1, y1, x2, y2, xc, yc;

    x1 = PreviousPoint.x;
    y1 = PreviousPoint.y;
    x2 = CurrentPoint.x;
    y2 = CurrentPoint.y;
    xc = CenterPoint.x;
    yc = CenterPoint.y;

    angle += atan((y2-yc)/(x2-xc)) - atan((y1-yc)/(x1-xc));
    if (x1 - xc < 0) {
        angle -= pi;
    }
    if (x2 - xc < 0) {
        angle += pi;
    }
    //CurrentAngle = pi/3 + angle;
    //RefreshDisplay();
    ClearWindow();
    DrawCurve(order_in_use, Order);
}

void zoom(void)
{
    if (ZoomType == ZOOM_IN) {
        EdgeLength *= 1 - SIZE_FACTOR;
    } else {
        EdgeLength *= 1 + SIZE_FACTOR;
    }
    ClearWindow();
    DrawCurve(order_in_use, Order);
}