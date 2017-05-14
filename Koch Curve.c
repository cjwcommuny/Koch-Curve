#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "newgraph.h"

#define DEFAULT_ORDER 3

struct Point {
    double x;
    double y;
};

void TimerEventProcess(int timerID);
void MouseEventProcess(int x, int y, int button, int event);

void GetInfo(void);
void DrawCurve(void);
void DrawForward(int *order);
void InitCurvePosition(void);
void RefreshDisplay(void);
void rotate(void);

static bool isRightMouseDown;
static struct Point CurrentPoint, PreviousPoint, CenterPoint;
static int Order;
static int *order_in_use;
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
}

void GetInfo(void)
{
    //char temp;

    order_in_use = GetBlock(sizeof(int));

    InitConsole();
    printf("Input the order of Koch Curve:\n");
    scanf("%d", &Order);
    getchar();
    //*order_in_use = Order;
    printf("Input the edge length of the original curve:\n");
    scanf("%lf", &EdgeLength);
    DrawCurve();
    //printf("length:%f\n", EdgeLength);
    //printf("Let's draw!\n");
    //FreeConsole();
}

void DrawCurve(void)
{
    InitCurvePosition();
    //printf("TEST:%d, %d, %f\n", Order, *order_in_use, EdgeLength);
    DrawForward(order_in_use);
    turn(-2*pi/3);
    *order_in_use = Order;
    DrawForward(order_in_use);
    turn(-2*pi/3);
    *order_in_use = Order;
    DrawForward(order_in_use);
}

void InitCurvePosition(void)
{
    double x0;// = GetWindowWidth() / 2 - EdgeLength / 2;
    double y0;// = GetWindowHeight() / 2 - EdgeLength / 2 / 1.732; //sqrt(3)
    double radius;

    //printf("centerpoint: %f, %f\n", CenterPoint.x, CenterPoint.y);
    radius = EdgeLength / 1.732; //sqrt(3)
    x0 = CenterPoint.x - radius * cos(angle + pi/6);
    y0 = CenterPoint.y - radius * sin(angle + pi/6);
    CurrentAngle = angle + pi/3;
    *order_in_use = Order;
    MovePen(x0, y0);
    //printf("%f, %f; %f, %f; %f, %f\n", CenterPoint.x, CenterPoint.y, x0, y0, CurrentAngle, angle);
}

void DrawForward(int *order)
{
    int temp;

    if (*order == 0) {
        forward(EdgeLength / pow(3, Order));
        //printf("finish\n");
    } else {
        //printf("here\n");
        (*order)--;
        temp = *order;
        DrawForward(order);
        turn(pi/3);
        *order = temp;
        DrawForward(order);
        turn(-2*pi/3);
        *order = temp;
        DrawForward(order);
        turn(pi/3);
        *order = temp;
        DrawForward(order);
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
            break;
        case ROLL_DOWN:
            break;
    }
}

void TimerEventProcess(int timerID)
{

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
    DrawCurve();
}