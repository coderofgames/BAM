// BAM.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "my_matrix.h"
#include "Utils.h"

#include <iostream>

using std::cout;
using std::endl;

float sign(float x)
{
	if (x > 0.0f) return 1.0f;
	if (x < 0.0f) return -1.0f;
	
	//if (x == 0.0f) 
	return x;
}

matrix& sign(matrix& m )
{
	for (int i = 0; i < m.NumRows(); i++)
		for (int j = 0; j < m.NumColumns(); j++)
			m(i, j) = sign(m(i, j));

	return m;
}

int main(int argc, char* argv[])
{
	// set A
	matrix x1_(6, 1);
	matrix x2_(6, 1);
	matrix x3_(6, 1);
	matrix x4_(6, 1);

	for (int i = 0; i < 6; i++) x1_(i, 0) = 1.0f;

	for (int i = 0; i < 6; i++) x2_(i, 0) = -1.0f;

	for (int i = 0; i < 6; i++) x3_(i, 0) = 1.0f;

	x3_(2, 0) = x3_(3, 0) = -1.0f;

	for (int i = 0; i < 6; i++) x4_(i, 0) = -1.0f;

	x4_(2, 0) = x4_(3, 0) = 1.0f;

	// set B
	matrix y1_(3, 1);
	matrix y2_(3, 1);
	matrix y3_(3, 1);
	matrix y4_(3, 1);

	for (int i = 0; i < 3; i++) y1_(i, 0) = 1.0f;

	for (int i = 0; i < 3; i++) y2_(i, 0) = -1.0f;

	for (int i = 0; i < 3; i++) y3_(i, 0) = 1.0f;

	y3_(1, 0) = -1.0f;

	for (int i = 0; i < 3; i++) y4_(i, 0) = -1.0f;

	y4_(1, 0) = 1.0f;

	// transpose y
	y1_.transpose();
	y2_.transpose();
	y3_.transpose();
	y4_.transpose();

	matrix weight_matrix = x1_ * y1_ + x2_ * y2_ + x3_ * y3_ + x4_ * y4_;

	weight_matrix.transpose();

	// first retrieve all y's
	matrix Y1 = sign(weight_matrix * x1_);
	Y1.transpose();

	matrix Y2 = sign(weight_matrix * x2_);
	Y2.transpose();

	matrix Y3 = sign(weight_matrix * x3_);
	Y3.transpose();

	matrix Y4 = sign(weight_matrix * x4_);
	Y4.transpose();

	if (y1_.IsEqual(Y1) &&
		y2_.IsEqual(Y2) &&
		y3_.IsEqual(Y3) &&
		y4_.IsEqual(Y4))
		cout << "all Y associations retrieved from x's" << endl;

	y1_.transpose();
	y2_.transpose();
	y3_.transpose();
	y4_.transpose();

	weight_matrix.transpose();

	// first retrieve all X's
	matrix X1 = sign(weight_matrix * y1_);
	//X1.transpose();


	matrix X2 = sign(weight_matrix * y2_);
	//X2.transpose();


	matrix X3 = sign(weight_matrix * y3_);
	//X3.transpose();

	matrix X4 = sign(weight_matrix * y4_);
	//X4.transpose();

	if (x1_.IsEqual(X1) &&
		x2_.IsEqual(X2) &&
		x3_.IsEqual(X3) &&
		x4_.IsEqual(X4))
		cout << "all X associations retrieved from y's" << endl;

	cout << "testing corrupted input memory 1" << endl;
	matrix X = x1_;
	X(0, 0) = -1;

	weight_matrix.transpose();

	// first retrieve all y's
	matrix Y = sign(weight_matrix * X);

	weight_matrix.transpose();

	matrix X_solved = sign(weight_matrix * Y);

	if (X_solved.IsEqual(x1_))
		cout << "corrupt matrix retrieval version 1 complete." << endl;
	else
	{
		X_solved.transpose();
		if (X_solved.IsEqual(x1_))
			cout << "corrupt matrix retrieval version 1 complete (after transposition)." << endl;

		X_solved.print();
	}

	matrix X_weird = x3_;
	for (int i = 0; i < X_weird.NumRows(); i++)
	{
		for (int j = 0; j < X_weird.NumColumns(); j++)
		{
			float rand = RandomFloat(0, 10);
			if (rand > 5.0)
			{
				float perturb = RandomFloat(-1, 1);
				X_weird(i, j) = -perturb;
			}
		}

	}
	X_weird.print();

	cout << endl;

	cout << "Corrupt Matrix Retrieval Test 2... " << endl;

	matrix Y_last_iteration;
	matrix X_last_iteration;

	weight_matrix.transpose();

	// first retrieve all y's
	matrix curr_Y = sign(weight_matrix * X_weird);
	//curr_Y.print();
	while (!curr_Y.IsEqual(Y_last_iteration) && !X_weird.IsEqual(X_last_iteration))
	{
		weight_matrix.transpose();
		X_last_iteration = X_weird;
		X_weird = sign(weight_matrix * curr_Y);

		weight_matrix.transpose();
		Y_last_iteration = curr_Y;
		curr_Y = sign(weight_matrix * X_weird);



	}
	cout << "X" << endl;
	X_weird.print();
	cout << "Y" << endl;
	curr_Y.print();
	return 0;
}

