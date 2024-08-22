#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<iostream>
#include<vector>
#include<functional>
#include<stack> 
#include<algorithm>
#include<math.h>
using namespace std;

boolean isDrawing, isErasing, isLining, isDrawingRect, isDrawCircle = false;
boolean pencilTool = false;
boolean eraserTool = false;
boolean lineTool = false;
boolean paintTool = false;
boolean rectTool = false;
boolean circleTool = false;
GLfloat currentColorArray[3];

vector <pair<float, float>> points;
vector <function<void()>> functionsToCall;
vector <pair<float, float>> eraserCurves;
vector <pair<float, float>> lineCurves(2);
vector <pair<float, float>> lineCurves1(2);
vector <pair<float, float>> paintedPoints;
vector <pair<float, float>> rectPoints;
vector <pair<float, float>> circlePoints(181);
vector <pair<float, float>> circlePoints2(181);


GLubyte targetColor[3];

void floodFillStack(int x, int y, GLfloat fillColor[3], GLubyte targetColor[3])
{
    stack<pair<int, int>> pointsToProcess;
    pointsToProcess.push(make_pair(x, y));

    bool visited[glutGet(GLUT_WINDOW_WIDTH)][glutGet(GLUT_WINDOW_HEIGHT)];
    fill(&visited[0][0], &visited[0][0] + sizeof(visited), false);

    while (!pointsToProcess.empty())
    {
        pair<int, int> current = pointsToProcess.top();
        pointsToProcess.pop();

        x = current.first;
        y = current.second;
        if (x < 0 || x >= glutGet(GLUT_WINDOW_WIDTH) || y < 0 || y >= glutGet(GLUT_WINDOW_HEIGHT) || visited[x][y] == true)
        {
            continue;
        }
        visited[x][y] = true;
        GLubyte pixelColor[3];
        glReadPixels(x, glutGet(GLUT_WINDOW_HEIGHT) - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixelColor);

        if (pixelColor[0] == targetColor[0] && pixelColor[1] == targetColor[1] && pixelColor[2] == targetColor[2])
        {
            float jPosX = (2.0f * static_cast<float>(x) / glutGet(GLUT_WINDOW_WIDTH)) - 1.0f;
            float jPosY = 1.0f - (2.0f * static_cast<float>(y) / glutGet(GLUT_WINDOW_HEIGHT));
            paintedPoints.push_back(make_pair(jPosX, jPosY));

            glFlush();

            pointsToProcess.push(make_pair(x + 1, y));
            pointsToProcess.push(make_pair(x, y + 1));
            pointsToProcess.push(make_pair(x - 1, y));
            pointsToProcess.push(make_pair(x, y - 1));
        }

    }

}

void drawlinerealtime()
{
    glColor3f(currentColorArray[0], currentColorArray[1], currentColorArray[2]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);

    for (const auto point : points)
    {
        glVertex2f(point.first, point.second);
    }

    glEnd();
}

void drawline(vector <pair<float, float>> tempPoints, const float* tempColorarr)
{

    glColor3f(tempColorarr[0], tempColorarr[1], tempColorarr[2]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);

    for (const auto point : tempPoints)
    {
        glVertex2f(point.first, point.second);
    }

    glEnd();
}


void EraserToolRealTime()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(20.0f);
    glBegin(GL_POINTS);
    for (const auto era : eraserCurves)
    {
        glVertex2f(era.first, era.second);
    }
    glEnd();
}

void EraseTool(vector<pair<float, float>> tempEraserCurve)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(20.0f);
    glBegin(GL_POINTS);

    for (const auto era : tempEraserCurve)
    {
        glVertex2f(era.first, era.second);
    }

    glEnd();
}

void lineToolDraw(vector<pair<float, float>> tempLinePoints, const float* tempColorArr7)
{
    glColor3f(tempColorArr7[0], tempColorArr7[1], tempColorArr7[3]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(tempLinePoints[0].first, tempLinePoints[0].second);
    glVertex2f(tempLinePoints[1].first, tempLinePoints[1].second);
    glEnd();
}

void lineToolDrawRealTime()
{
    glColor3f(currentColorArray[0], currentColorArray[1], currentColorArray[2]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(lineCurves[0].first, lineCurves[0].second);
    glVertex2f(lineCurves[1].first, lineCurves[1].second);
    glEnd();
}

void rectangleToolRealtime()
{
    glColor3f(currentColorArray[0], currentColorArray[1], currentColorArray[2]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);

    for (const auto point : rectPoints)
    {
        glVertex2f(point.first, point.second);
    }

    glEnd();
}

void rectangleTool(vector<pair<float, float>> tempRectPoints, const float* tempColorarr12)
{
    glColor3f(tempColorarr12[0], tempColorarr12[1], tempColorarr12[2]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);

    for (const auto point : tempRectPoints)
    {
        glVertex2f(point.first, point.second);
    }

    glEnd();
}

void circleToolRealtime()
{
    glColor3f(currentColorArray[0], currentColorArray[1], currentColorArray[2]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);

    for (int i = 1; i <= 180; i++)
    {
        glVertex2f(circlePoints[i].first, circlePoints[i].second);
    }
    glVertex2f(circlePoints[1].first, circlePoints[1].second);

    glEnd();
}

void circleDrawTool(vector<pair<float, float>> tempCirclePoints, const float* tempColorarr13)
{
    glColor3f(tempColorarr13[0], tempColorarr13[1], tempColorarr13[2]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);

    for (int i = 1; i <= 180; i++)
    {
        glVertex2f(tempCirclePoints[i].first, tempCirclePoints[i].second);
    }
    glVertex2f(tempCirclePoints[1].first, tempCirclePoints[1].second);

    glEnd();
}

void setBool(bool eraser, bool pencil, bool line, bool paint, bool rect, bool circ)
{
    pencilTool = pencil;
    eraserTool = eraser;
    lineTool = line;
    paintTool = paint;
    rectTool = rect;
    circleTool = circ;
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'E':
    case 'e':
        setBool(true, false, false, false, false, false);
        break;
    case 'P':
    case 'p':
        setBool(false, true, false, false, false, false);
        break;
    case 'R':
    case 'r':
        setBool(false, false, false, false, true, false);
        break;
    case 'L':
    case 'l':
        setBool(false, false, true, false, false, false);
        break;
    case 'F':
    case 'f':
        setBool(false, false, false, true, false, false);
        break;
    case 'C':
    case 'c':
        setBool(false, false, false, false, false, true);
        break;
    case 'G':       // green
    case 'g':
        currentColorArray[0] = 0.0f;
        currentColorArray[1] = 1.0f;
        currentColorArray[2] = 0.0f;
        break;
    case 'B':       // brown
    case 'b':
        currentColorArray[0] = 150.0f / 255.0f;
        currentColorArray[1] = 75.0f / 255.0f;
        currentColorArray[2] = 0.0f;
        break;
    case 'K':   // black
    case 'k':
        currentColorArray[0] = 0.0f;
        currentColorArray[1] = 0.0f;
        currentColorArray[2] = 0.0f;
        break;
    case 'Y':       // yellow
    case 'y':
        currentColorArray[0] = 1.0f;
        currentColorArray[1] = 1.0f;
        currentColorArray[2] = 0.0f;
        break;

    case 'Z':
    case 'z':
        if (!functionsToCall.empty())
        {
            functionsToCall.pop_back();
        }
        glutPostRedisplay();
        break;
    }
}

void handleMouseClick(int button, int state, int x, int y)
{
    float posX = (2.0f * static_cast<float>(x) / glutGet(GLUT_WINDOW_WIDTH)) - 1.0f;
    float posY = 1.0f - (2.0f * static_cast<float>(y) / glutGet(GLUT_WINDOW_HEIGHT));
    // cerr << "NDC x: " << posX << endl;
    // cerr << "NDC y: " << posY << endl;

    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN) {
            if (pencilTool)
            {
                points.push_back(make_pair(posX, posY));
                isDrawing = true;
            }
            if (eraserTool)
            {
                eraserCurves.push_back(make_pair(posX, posY));
                isErasing = true;
            }
            if (lineTool)
            {
                lineCurves[0] = make_pair(posX, posY);
                lineCurves[1] = make_pair(posX, posY);
                isLining = true;
            }
            if (paintTool)
            {
                glReadPixels(x, glutGet(GLUT_WINDOW_HEIGHT) - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, targetColor);
                floodFillStack(x, y, currentColorArray, targetColor);

                vector<pair<float, float>> tempPaintedPoints = paintedPoints;
                float tempPaintColorArray[3];
                for (int i = 0; i < 3; i++)
                {
                    tempPaintColorArray[i] = currentColorArray[i];
                }

                functionsToCall.push_back([tempPaintedPoints, tempPaintColorArray] {
                    float colorPaintCopy[3] = { tempPaintColorArray[0], tempPaintColorArray[1], tempPaintColorArray[2] };
                    drawline(tempPaintedPoints, colorPaintCopy);
                    }
                );
                paintedPoints.clear();
            }

            if (rectTool)
            {
                rectPoints.push_back(make_pair(posX, posY));
                rectPoints.push_back(make_pair(posX, posY));
                rectPoints.push_back(make_pair(posX, posY));
                rectPoints.push_back(make_pair(posX, posY));
                rectPoints.push_back(make_pair(posX, posY));
                isDrawingRect = true;
            }

            if (circleTool)
            {
                isDrawCircle = true;
                for (int i = 0; i <= 180; i++)
                {
                    circlePoints[i] = make_pair(posX, posY);
                }
            }
        }

        else if (state == GLUT_UP) {
            if (pencilTool)
            {
                isDrawing = false;
                vector<pair<float, float>> tempPoints = points;
                float tempColorarr[3];
                for (int i = 0; i < 3; ++i)
                {
                    tempColorarr[i] = currentColorArray[i];
                }

                functionsToCall.push_back([tempPoints, tempColorarr] {
                    float colorCopy[3] = { tempColorarr[0], tempColorarr[1], tempColorarr[3] };
                    drawline(tempPoints, colorCopy);
                    }
                );

                points.clear();
            }

            if (eraserTool)
            {
                isErasing = false;
                vector<pair<float, float>> tempErasePoints = eraserCurves;
                functionsToCall.push_back([tempErasePoints] {
                    EraseTool(tempErasePoints);
                    }
                );
                eraserCurves.clear();
            }

            if (lineTool)
            {
                isLining = false;
                vector<pair<float, float>> tempLinePoints = lineCurves;
                float tempColorarr6[3];
                for (int i = 0; i < 3; ++i)
                {
                    tempColorarr6[i] = currentColorArray[i];
                }

                functionsToCall.push_back([tempLinePoints, tempColorarr6] {
                    float colorCopy7[3] = { tempColorarr6[0], tempColorarr6[1], tempColorarr6[3] };
                    lineToolDraw(tempLinePoints, colorCopy7);
                    }
                );

                lineCurves = lineCurves1;
            }

            if (rectTool)
            {

                rectPoints[1].first = rectPoints[0].first;
                rectPoints[1].second = posY;
                rectPoints[2].first = posX;
                rectPoints[2].second = posY;
                rectPoints[3].first = posX;
                rectPoints[3].second = rectPoints[0].second;
                rectPoints[4].first = rectPoints[0].first;
                rectPoints[4].second = rectPoints[0].second;

                isDrawingRect = false;
                vector<pair<float, float>> tempRectanglePoints = rectPoints;
                float tempColorarr12[3];
                for (int i = 0; i < 3; ++i)
                {
                    tempColorarr12[i] = currentColorArray[i];
                }

                functionsToCall.push_back([tempRectanglePoints, tempColorarr12] {
                    float colorCopy12[3] = { tempColorarr12[0], tempColorarr12[1], tempColorarr12[3] };
                    rectangleTool(tempRectanglePoints, colorCopy12);
                    }
                );

                rectPoints.clear();
            }
            if (circleTool)
            {
                float r = sqrt(pow((circlePoints[0].first - posX), 2) + pow((circlePoints[0].second - posY), 2));
                // circlePoints = circlePoints2;

                for (int i = 1; i <= 180; i++)
                {
                    float theta = 2.0f * acos(-1) * static_cast<float>(i) / static_cast <float>(180);
                    circlePoints[i].first = r * cos(theta) + circlePoints[0].first;
                    circlePoints[i].second = r * sin(theta) + circlePoints[0].second;
                }


                isDrawCircle = false;
                vector <pair<float, float>> tempCirclePoints(181);
                tempCirclePoints = circlePoints;

                float tempColorarr13[3];
                for (int i = 0; i < 3; ++i)
                {
                    tempColorarr13[i] = currentColorArray[i];
                }

                functionsToCall.push_back([tempCirclePoints, tempColorarr13] {
                    float colorCopy12[3] = { tempColorarr13[0], tempColorarr13[1], tempColorarr13[3] };
                    circleDrawTool(tempCirclePoints, colorCopy12);
                    }
                );

                circlePoints = circlePoints2;
            }

            glutPostRedisplay();
        }

    }
}

void handleMouseMotion(int x, int y)
{
    float currentX = (2.0f * static_cast<float>(x) / glutGet(GLUT_WINDOW_WIDTH)) - 1.0f;
    float currentY = 1.0f - (2.0f * static_cast<float>(y) / glutGet(GLUT_WINDOW_HEIGHT));

    if (pencilTool && isDrawing)
    {
        points.push_back(make_pair(currentX, currentY));
    }
    if (eraserTool && isErasing)
    {
        eraserCurves.push_back(make_pair(currentX, currentY));
    }
    if (lineTool && isLining)
    {
        lineCurves[1] = make_pair(currentX, currentY);
    }
    if (rectTool && isDrawingRect)
    {
        rectPoints[1].first = rectPoints[0].first;
        rectPoints[1].second = currentY;
        rectPoints[2].first = currentX;
        rectPoints[2].second = currentY;
        rectPoints[3].first = currentX;
        rectPoints[3].second = rectPoints[0].second;
        rectPoints[4].first = rectPoints[0].first;
        rectPoints[4].second = rectPoints[0].second;
    }

    if (circleTool && isDrawCircle)
    {
        float r = sqrt(pow((circlePoints[0].first - currentX), 2) + pow((circlePoints[0].second - currentY), 2));
        // circlePoints = circlePoints2;

        for (int i = 1; i <= 180; i++)
        {
            float theta = 2.0f * acos(-1) * static_cast<float>(i) / static_cast <float>(180);
            circlePoints[i].first = r * cos(theta) + circlePoints[0].first;
            circlePoints[i].second = r * sin(theta) + circlePoints[0].second;
        }


        // for (int i = 0; i < 100; i++) {
        //     float theta = 2.0f * PI * static_cast<float>(i) / static_cast<float>(100);
        //     float x = radii * cos(theta);
        //     float y = radii * sin(theta);

        //     // Normalize the coordinates based on the aspect ratio
        //     x /= aspect_ratioii;

        //     circleVector.push_back(std::make_pair(centreofX + x, centreofY + y));

    }




    glutPostRedisplay();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 1.0f);
    for (const auto& func : functionsToCall)
    {
        func();
    }
    drawlinerealtime();
    EraserToolRealTime();
    lineToolDrawRealTime();
    rectangleToolRealtime();
    circleToolRealtime();
    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    currentColorArray[0] = 0.0f;
    currentColorArray[1] = 0.0f;
    currentColorArray[2] = 0.0f;

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Paint App");
    glutPositionWindow(100, 50);
    glutDisplayFunc(display);
    glutMouseFunc(handleMouseClick);
    glutMotionFunc(handleMouseMotion);
    glutKeyboardFunc(keyboard);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glutMainLoop();

    return 0;
}
