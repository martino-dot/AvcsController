#pragma once

///////////////////////////////////////////////////////////////////////////////
// These macros are used to make code usable both on the Arduino itself (as
// a self-test that runs at startup) and on a Win32 PC (as unit tests that can
// be used to validate code before flashing it to an Arduino).
///////////////////////////////////////////////////////////////////////////////
#ifdef ARDUINO
#define CLEAR() lcd.clear();
#define SETCURSOR(x,y) lcd.setCursor(x,y)
#define PRINT(x) lcd.print(x)
#define DELAY(x) delay(x)
#else
#include "stdafx.h"
#define CLEAR() 
#define SETCURSOR(x,y) printf("\r\n")
#define PRINT(x) printf(x)
#define DELAY(x)
#endif

extern char FailureMessage[];

///////////////////////////////////////////////////////////////////////////////
// Invokes a test case method, passing both the method's name and its function 
// pointer to the test harness.
///////////////////////////////////////////////////////////////////////////////
#define InvokeTest(x) InvokeTestCase(#x, Test##x);

///////////////////////////////////////////////////////////////////////////////
// Run a single test
///////////////////////////////////////////////////////////////////////////////
bool InvokeTestCase(const char *name, bool(*test)(void));

///////////////////////////////////////////////////////////////////////////////
// This can be handy when debugging tests on Arduino
///////////////////////////////////////////////////////////////////////////////
void ShowProgress(char* msg, int step);

///////////////////////////////////////////////////////////////////////////////
// Run tests for all features
///////////////////////////////////////////////////////////////////////////////
void SelfTest();

///////////////////////////////////////////////////////////////////////////////
// This is so meta.
///////////////////////////////////////////////////////////////////////////////
void SelfTestSelfTest();

///////////////////////////////////////////////////////////////////////////////
// Tools to validate test results
///////////////////////////////////////////////////////////////////////////////
bool CompareStrings(char* expected, char *actual);
bool CompareUnsigned(unsigned actual, unsigned expected, const char *message);

///////////////////////////////////////////////////////////////////////////////
// Compare two numbers, with 1% tolerance
///////////////////////////////////////////////////////////////////////////////
template <typename T>
bool WithinOnePercent(T actual, T expected, char* message)
{
	float upperBound = (expected * 101.0f) / 100.0f;
	if (actual > upperBound)
	{
		sprintf(FailureMessage, "%s Hi", message);
		return false;
	}

	float lowerBound = (expected * 99.0f) / 100.0f;
	if (actual < lowerBound)
	{
		sprintf(FailureMessage, "%s Lo", message);
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Compare two numbers, with 10% tolerance
///////////////////////////////////////////////////////////////////////////////
template <typename T>
bool WithinTenPercent(T actual, T expected, char* message)
{
	unsigned upperBound = (expected * 110) / 100;
	if (actual > upperBound)
	{
		sprintf(FailureMessage, "%s Hi", message);
		return false;
	}

	unsigned lowerBound = (expected * 90) / 100;
	if (actual < lowerBound)
	{
		sprintf(FailureMessage, "%s Lo", message);
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
// To invoke when a test fails
///////////////////////////////////////////////////////////////////////////////
void TestFailed(char *message);
void TestFailed(char *message1, char* message2);
