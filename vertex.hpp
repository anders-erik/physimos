



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

	// Row 1
	simObject->transformMatrixRowMajor[0] = simObject->scale.x * cos(simObject->rotation.z);
	simObject->transformMatrixRowMajor[1] = - simObject->scale.x * sin(simObject->rotation.z);
	simObject->transformMatrixRowMajor[2] = 0.0f;
	simObject->transformMatrixRowMajor[3] = simObject->translation.x;

	// Row 2
	simObject->transformMatrixRowMajor[4] = simObject->scale.y * sin(simObject->rotation.z);
	simObject->transformMatrixRowMajor[5] = simObject->scale.y * cos(simObject->rotation.z);
	simObject->transformMatrixRowMajor[6] = 0.0f;
	simObject->transformMatrixRowMajor[7] = simObject->translation.y;

	// Row 3
	simObject->transformMatrixRowMajor[8] = 0.0f;
	simObject->transformMatrixRowMajor[9] = 0.0f;
	simObject->transformMatrixRowMajor[10] = simObject->scale.z;
	simObject->transformMatrixRowMajor[11] = simObject->translation.z;

	// Row 4
	simObject->transformMatrixRowMajor[12] = 0.0f;
	simObject->transformMatrixRowMajor[13] = 0.0f;
	simObject->transformMatrixRowMajor[14] = 0.0f;
	simObject->transformMatrixRowMajor[15] = 1.0f;

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