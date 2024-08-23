



// Simulation Object methods
void RotationSimObject(struct SimObject * simObject, Point3 rotationVector);
void SetScaleSimObject(struct SimObject * simObject, Point3 scaleVector);
void MoveSimObject(struct SimObject * simObject, Point3 moveVector);
void SetSimObjectTranform(struct SimObject * simObject);



// END HEADER DECLARATION
// ---------------------------------

#include <cmath>





// Simulation Object

void RotationSimObject(struct SimObject * simObject, Point3 rotationVector){
	simObject->rotation.x += rotationVector.x;
	simObject->rotation.y += rotationVector.y;
	simObject->rotation.z += rotationVector.z;
}

void SetRotationSimObject(struct SimObject * simObject, Point3 rotationVector){
	simObject->rotation.x = rotationVector.x;
	simObject->rotation.y = rotationVector.y;
	simObject->rotation.z = rotationVector.z;
}

void SetScaleSimObject(struct SimObject * simObject, Point3 scaleVector){
	simObject->scale.x = scaleVector.x;
	simObject->scale.y = scaleVector.y;
	simObject->scale.z = scaleVector.z;
}

void MoveSimObject(struct SimObject * simObject, Point3 moveVector){
	simObject->translation.x += moveVector.x;
	simObject->translation.y += moveVector.y;
	simObject->translation.z += moveVector.z;
}

void SetPositionSimObject(struct SimObject * simObject, Point3 moveVector){
	simObject->translation.x = moveVector.x;
	simObject->translation.y = moveVector.y;
	simObject->translation.z = moveVector.z;
}

void SetSimObjectTranform(struct SimObject * simObject){

	float Sx = simObject->scale.x;
	float Sy = simObject->scale.y;
	float Sz = simObject->scale.z;

	float Tx = simObject->translation.x;
	float Ty = simObject->translation.y;
	float Tz = simObject->translation.z;

	float c_ph = cos(simObject->rotation.x);
	float s_ph = sin(simObject->rotation.x);
	float c_th = cos(simObject->rotation.y);
	float s_th = sin(simObject->rotation.y);
	float c_ps = cos(simObject->rotation.z);
	float s_ps = sin(simObject->rotation.z);

	float a11 =  c_ps*c_ph - s_ps*c_th*s_ph;
	float a12 = -c_ps*s_ph - s_ps*c_th*c_ph;
	float a13 =  s_ps*s_th;
	float a14 = 0;

	float a21 =  s_ps*c_ph + c_ps*c_th*s_ph; 
	float a22 = -s_ps*s_ph + c_ps*c_th*c_ph;
	float a23 = -c_ps*s_th;
	float a24 = 0;

	float a31 =  s_th*s_ph; 
	float a32 =  s_th*c_ph;
	float a33 =  c_th;
	float a34 = 0;

	float a41 =  0; 
	float a42 =  0;
	float a43 =  0;
	float a44 =  1;


	// Row 1
	simObject->transformMatrixRowMajor[0] = Sx * (a11 + Tx*a41);
	simObject->transformMatrixRowMajor[1] = Sy * (a12 + Tx*a42);
	simObject->transformMatrixRowMajor[2] = Sz * (a13 + Tx*a43);
	simObject->transformMatrixRowMajor[3] = 	  a14 + Tx*a44;

	// Row 2
	simObject->transformMatrixRowMajor[4] =  Sx * (a21 + Ty*a41);
	simObject->transformMatrixRowMajor[5] =  Sy * (a22 + Ty*a42);
	simObject->transformMatrixRowMajor[6] =  Sz * (a23 + Ty*a43);
	simObject->transformMatrixRowMajor[7] =  	   a24 + Ty*a44;;

	// Row 3
	simObject->transformMatrixRowMajor[8] =   Sx * (a31 + Tz*a41);
	simObject->transformMatrixRowMajor[9] =   Sy * (a32 + Tz*a42);
	simObject->transformMatrixRowMajor[10] =  Sz * (a33 + Tz*a43);
	simObject->transformMatrixRowMajor[11] =  	    a34 + Tz*a44;;

	// Row 4
	simObject->transformMatrixRowMajor[12] =  Sx * a41;
	simObject->transformMatrixRowMajor[13] =  Sy * a42;
	simObject->transformMatrixRowMajor[14] =  Sx * a43;
	simObject->transformMatrixRowMajor[15] =  	   a44;

}




// OLD BELOW
// ------------------------------------


void moveVerticies(float *verticies, int vertexCount);
void scale16Transform(float * transform16, float dx, float dy, float dz);
void setSimplePerspective(float * transformMatrix, float * perspectiveMatrix, float ZF, float ZN);
void translate16Transform(float * transform16, float dx, float dy, float dz);
float *  transposeToShader16(float * transform16, float * transformShader16);


void moveVerticies(float *verticies, int vertexCount)
{
	for (int i = 0; i < vertexCount; i++)
	{
		verticies[i] = verticies[i] + 0.01;
	}
}

void scale16Transform(float * transform16, float dx, float dy, float dz){
	transform16[0]	*= dx;
	transform16[5]	*= dy;
	transform16[10]	*= dz;
}

void translate16Transform(float * transform16, float dx, float dy, float dz){
	transform16[3] += dx;
	transform16[7] += dy;
	transform16[11] += dz;
}

void setSimplePerspective(float * transformMatrix, float * perspectiveMatrix, float ZF, float ZN){
	// Original : sets all z-values to model center ...
	// perspectiveMatrix[0] = ZN / transformMatrix[11];
	// perspectiveMatrix[5] = ZN / transformMatrix[11];
	// perspectiveMatrix[10] = (ZN - transformMatrix[11]) / (ZF - ZN);

	// Original : defined per drawing "minimal-projection.png", now with perspective dividing in mind
	perspectiveMatrix[0] = ZN;
	perspectiveMatrix[5] = ZN;
	perspectiveMatrix[10] = -1.0f / (ZF - ZN);
	perspectiveMatrix[11] = ZN / (ZF - ZN); 
	perspectiveMatrix[14] = 1.0f; 

	//http://www.songho.ca/opengl/gl_projectionmatrix.html
	// perspectiveMatrix[0] = ZN;
	// perspectiveMatrix[5] = ZN;
	// perspectiveMatrix[10] = -(ZF + ZN) / (ZF - ZN); // NOT NEEDED??
	// perspectiveMatrix[11] = 2*ZF*ZN / (ZF - ZN);
	// perspectiveMatrix[14] = 1.0f; 
}

// 
float *  transposeToShader16(float * transform16, float * transformShader16){

	for(int c = 0; c < 4; c++){ // shader format column
		for(int r = 0; r < 4; r++){ // 
			transformShader16[c*4 + r] = transform16[r*4 + c];
		}
	}
	return transformShader16;

}