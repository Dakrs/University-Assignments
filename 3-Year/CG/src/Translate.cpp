


#include "../headers/Translate.h"

void buildRotMatrix(float *x, float *y, float *z, float *m) {

    m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
    m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
    m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
    m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}


void cross(float *a, float *b, float *res) {

    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];

}


void normalize(float *a) {

    float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0]/l;
    a[1] = a[1]/l;
    a[2] = a[2]/l;
}


float length(float *v) {

    float res = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    return res;

}

void multMatrixVector(float *m, float *v, float *res) {

    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j * 4 + k];
        }
    }

}

Translate::Translate(float t) : Transformation() {
    control_points = new vector<Point*>();
    duration = t;
}

Translate::Translate(Translate * t) : Transformation() {
    control_points = new vector<Point*>();
    vector<Point *>::iterator it1;
    for(it1 = (t->control_points)->begin(); it1 != (t->control_points)->end(); it1++){
        Point * p = (*it1);
        control_points->push_back(new Point (p->getX(),p->getY(),p->getZ()));
    }
    duration = t->duration;
}

Translate::~Translate() {};

Transformation* Translate::clone() {
    return new Translate(this);
}

void Translate::addPoint(Point *p) {
    control_points->push_back(p);
}

void Translate::getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv) {

    // catmull-rom matrix
    float m[4][4] = {	{-0.5f,  1.5f, -1.5f,  0.5f},
                         { 1.0f, -2.5f,  2.0f, -0.5f},
                         {-0.5f,  0.0f,  0.5f,  0.0f},
                         { 0.0f,  1.0f,  0.0f,  0.0f}};

    //vector que vamos usar para guardar os pontos
    float p[4];

    //vector do eixo dos X
    float AxisX[4];

    //vector do eixo dos Y
    float AxisY[4];

    //vector do eixo dos Z
    float AxisZ[4];

    // Compute A = M * P
    // componente x
    p[0] = p0[0];
    p[1] = p1[0];
    p[2] = p2[0];
    p[3] = p3[0];

    //fazer multiplicação da matriz pelo vector e guardar no vector do eixo dos X
    multMatrixVector((float *)m, p, AxisX);

    // componente y
    p[0] = p0[1];
    p[1] = p1[1];
    p[2] = p2[1];
    p[3] = p3[1];

    //fazer multiplicação da matriz pelo vector e guardar no vector do eixo dos Y
    multMatrixVector((float *)m, p, AxisY);

    //componente z
    p[0] = p0[2];
    p[1] = p1[2];
    p[2] = p2[2];
    p[3] = p3[2];

    //fazer multiplicação da matriz pelo vector e guardar no vector do eixo dos Z
    multMatrixVector((float *)m, p, AxisZ);

    // Compute pos = T * A
    float t_vector[4] = {t*t*t, t*t, t, 1};
    pos[0] = t_vector[0]*AxisX[0] + t_vector[1]*AxisX[1] + t_vector[2]*AxisX[2] + t_vector[3]*AxisX[3];
    pos[1] = t_vector[0]*AxisY[0] + t_vector[1]*AxisY[1] + t_vector[2]*AxisY[2] + t_vector[3]*AxisY[3];
    pos[2] = t_vector[0]*AxisZ[0] + t_vector[1]*AxisZ[1] + t_vector[2]*AxisZ[2] + t_vector[3]*AxisZ[3];

    // compute deriv = T' * A
    float deriv_t_vector[4] = {3*t*t, 2*t, 1, 0};
    deriv[0] = deriv_t_vector[0]*AxisX[0] + deriv_t_vector[1]*AxisX[1] + deriv_t_vector[2]*AxisX[2] + deriv_t_vector[3]*AxisX[3];
    deriv[1] = deriv_t_vector[0]*AxisY[0] + deriv_t_vector[1]*AxisY[1] + deriv_t_vector[2]*AxisY[2] + deriv_t_vector[3]*AxisY[3];
    deriv[2] = deriv_t_vector[0]*AxisZ[0] + deriv_t_vector[1]*AxisZ[1] + deriv_t_vector[2]*AxisZ[2] + deriv_t_vector[3]*AxisZ[3];
}

void Translate::getGlobalCatmullRomPoint(float gt, float *pos, float *deriv) {

    float t = gt * control_points->size(); // this is the real global t
    int index = floor(t);  // which segment
    t = t - index; // where within  the segment

    // indices store the points
    int indices[4];
    indices[0] = (index + control_points->size()-1)%control_points->size();
    indices[1] = (indices[0]+1)%control_points->size();
    indices[2] = (indices[1]+1)%control_points->size();
    indices[3] = (indices[2]+1)%control_points->size();

    float p0[3],p1[3],p2[3],p3[3];

    control_points->at(indices[0])->get(p0);
    control_points->at(indices[1])->get(p1);
    control_points->at(indices[2])->get(p2);
    control_points->at(indices[3])->get(p3);


    getCatmullRomPoint(t, p0, p1, p2, p3, pos, deriv);
}

void Translate::renderCatmullRomCurve() {

// draw curve using line segments with GL_LINE_LOOP

    float pos[3];
    float deriv[3];

    glColor3f(1,1,1);
    glDisable(GL_LIGHTING);
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < 100; i++){
        getGlobalCatmullRomPoint((float) i/100, pos, deriv);
        glVertex3f(pos[0], pos[1], pos[2]);
    }
    glEnd();
    glEnable(GL_LIGHTING);

}

void Translate::perform() {
    if (control_points->size() < 4 || duration == 0){
        Point * p = control_points->at(0);
        glTranslatef(p->getX(),p->getY(),p->getZ());
        return;
    }

    renderCatmullRomCurve();

    int time = glutGet(GLUT_ELAPSED_TIME);
    float k = (time / 1000.0) / duration;
    float position = k - floor(k);

    float res[3],dev[3];
    getGlobalCatmullRomPoint(position, res, dev);
    glTranslatef(res[0],res[1],res[2]);

}