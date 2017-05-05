#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "newgraph.h"

#define DEFAULT_ORDER 3

void TimerEventProcess(int timerID);
void GetInfo(void);
void DrawCurve(void);
void DrawForward(int *order);
void InitCurvePosition(void);

static int Order;
static int *order_in_use;
static double EdgeLength = 3.00;
struct curveADT *curve;
extern double CurrentAngle;

void Main()
{
    SetWindowTitle("Koch Curve Program");
    InitGraphics();
    GetInfo();
    DrawCurve();
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
    *order_in_use = Order;
    printf("Input the edge length of the original curve:\n");
    scanf("%lf", &EdgeLength);
    //printf("length:%f\n", EdgeLength);
    //printf("Let's draw!\n");
    FreeConsole();
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
    double x0 = GetWindowWidth() / 2 - EdgeLength / 2;
    double y0 = GetWindowHeight() / 2 - EdgeLength / 2 / 1.732;
    MovePen(x0, y0);
    printf("%f, %f\n", x0, y0);
    CurrentAngle = pi/3;
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

void TimerEventProcess(int timerID)
{

}