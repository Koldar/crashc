/*
 * plotProducerTest.c
 *
 *  Created on: May 22, 2017
 *      Author: koldar
 */

#include <assert.h>
#include <stdbool.h>
#include "CuTest.h"
#include "plot2DProducer.h"
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


void testPlot2DProducer01(CuTest* tc) {
	Plot2DHelper* helper = initPlot2DHelper(__func__);

	addSeries(helper, "sin", PS_POINTS);

	for (double x=-10; x<+10; x += 0.1) {
		addPoint(helper, x, sin(x));
	}

	plotWith2DHelper(helper);
	destroyPlot2DHelper(helper);
}

///like ::testPlot2DProducer01 but uses ::addPoints with 2 series
void testPlot2DProducer02(CuTest* tc) {
	Plot2DHelper* helper = initPlot2DHelper(__func__);

	addSeries(helper, "sin", PS_POINTS);
	addSeries(helper, "cos", PS_POINTS);

	for (double x=-10; x<+10; x += 0.1) {
		addPoints(helper, x, (double[]){sin(x), cos(x)});
	}

	plotWith2DHelper(helper);
	destroyPlot2DHelper(helper);
}

///like ::testPlot2DProducer02 but set title, x axis and y axis
void testPlot2DProducer03(CuTest* tc) {
	Plot2DHelper* helper = initPlot2DHelper(__func__);

	addSeries(helper, "sin", PS_POINTS);
	addSeries(helper, "cos", PS_POINTS);

	for (double x=-10; x<+10; x += 0.1) {
		addPoints(helper, x, (double[]){sin(x), cos(x)});
	}

	setTitle(helper, __func__);
	setXLabel(helper, "x");
	setYLabel(helper, "f(x)");

	plotWith2DHelper(helper);
	destroyPlot2DHelper(helper);
}

void testPlot2DProducer04(CuTest* tc) {
	Plot2DHelper* helper = initPlot2DHelper(__func__);

	addSeries(helper, "sin", PS_LINES);
	addSeries(helper, "cos", PS_POINTS);

	for (double x=-10; x<+10; x += 0.1) {
		addPoint(helper, x, sin(x));
		addPoint(helper, x, cos(x));
	}

	setTitle(helper, __func__);
	setXLabel(helper, "x");
	setYLabel(helper, "f(x)");

	plotWith2DHelper(helper);
	destroyPlot2DHelper(helper);
}

void testPlot2DProducer05(CuTest* tc) {
	Plot2DHelper* helper = initPlot2DHelper(__func__);

	addSeries(helper, "sin", PS_LINES);

	for (double x=-10; x<+10; x += 0.1) {
		addPoint(helper, x, sin(x));
	}

	setTitle(helper, __func__);
	setXLabel(helper, "x");
	setYLabel(helper, "f(x)");

	plotWith2DHelper(helper);
	destroyPlot2DHelper(helper);
}

void testPlot2DProducer06(CuTest* tc) {
	Plot2DHelper* helper = initPlot2DHelper(__func__);

	addSeries(helper, "sin", PS_LINES);

	for (double x=-10; x<+10; x += 0.1) {
		addPoint(helper, x, sin(x));
	}

	setTitle(helper, __func__);
	setXLabel(helper, "x");
	setYLabel(helper, "f(x)");
	setGrid(helper, true);

	plotWith2DHelper(helper);
	destroyPlot2DHelper(helper);
}

///use the logaritmic scale on y
void testPlot2DProducer07(CuTest* tc) {
	Plot2DHelper* helper = initPlot2DHelper(__func__);

	addSeries(helper, "e^x", PS_LINES);

	for (double x=0; x<+10; x += 0.1) {
		addPoint(helper, x, exp(x));
	}

	setTitle(helper, __func__);
	setXLabel(helper, "x");
	setYLabel(helper, "f(x)");
	setGrid(helper, true);
	setYAxisType(helper, AT_LOGARITMIC);

	plotWith2DHelper(helper);
	destroyPlot2DHelper(helper);
}

///use the logaritmic scale on x
void testPlot2DProducer08(CuTest* tc) {
	Plot2DHelper* helper = initPlot2DHelper(__func__);

	addSeries(helper, "e^x", PS_LINES);

	for (double x=0; x<+10; x += 0.1) {
		addPoint(helper, x, exp(x));
	}

	setTitle(helper, __func__);
	setXLabel(helper, "x");
	setYLabel(helper, "f(x)");
	setGrid(helper, true);
	setXAxisType(helper, AT_LOGARITMIC);

	plotWith2DHelper(helper);
	destroyPlot2DHelper(helper);
}

///use the logaritmic scale on both x and y
void testPlot2DProducer09(CuTest* tc) {
	Plot2DHelper* helper = initPlot2DHelper(__func__);

	addSeries(helper, "e^x", PS_LINES);

	for (double x=0; x<+10; x += 0.1) {
		addPoint(helper, x, exp(x));
	}

	setTitle(helper, __func__);
	setXLabel(helper, "x");
	setYLabel(helper, "f(x)");
	setGrid(helper, true);
	setXAxisType(helper, AT_LOGARITMIC);
	setYAxisType(helper, AT_LOGARITMIC);

	plotWith2DHelper(helper);
	destroyPlot2DHelper(helper);
}

///use subtitle
void testPlot2DProducer10(CuTest* tc) {
	Plot2DHelper* helper = initPlot2DHelper(__func__);

	addSeries(helper, "e^x", PS_LINES);

	for (double x=0; x<+10; x += 0.1) {
		addPoint(helper, x, exp(x));
	}

	setTitle(helper, __func__);
	setSubtitle(helper, "an interesting function");
	setXLabel(helper, "x");
	setYLabel(helper, "f(x)");
	setGrid(helper, true);

	plotWith2DHelper(helper);
	destroyPlot2DHelper(helper);
}

void testPlots2DProducer01(CuTest* tc) {
	Plot2DHelper* helper = initPlot2DHelper(__func__);

	addSeries(helper, "e^x", PS_LINES);

	for (double x=0; x<+10; x += 0.1) {
		addPoint(helper, x, exp(x));
	}

	setTitle(helper, __func__);
	setSubtitle(helper, "an interesting function");
	setXLabel(helper, "x");
	setYLabel(helper, "f(x)");
	setGrid(helper, true);

	plotsWith2DHelper(helper, true);
	destroyPlot2DHelper(helper);
}

void testPlot2DProducer11(CuTest* tc) {
	Plot2DHelper* helper = initPlot2DHelper(__func__);

	addSeries(helper, "3*x", PS_LINES);
	addSeries(helper, "2*x", PS_LINES);

	for (double x=0; x<+10; x += 0.1) {
		addPoints(helper, x, (double[]){3*x, 2*x});
	}

	setTitle(helper, __func__);
	setSubtitle(helper, "an interesting functions");
	setXLabel(helper, "x");
	setYLabel(helper, "f(x)");
	setGrid(helper, true);
	setCreateDataCSVFileOfPlot2DProducer(helper, true);

	plotsWith2DHelper(helper, true);
	destroyPlot2DHelper(helper);

	//check if csv has been created
	char buffer[BUFFER_SIZE];
	sprintf(buffer, "%s.plotdata.csv", __func__);
	FILE* fcsv = fopen(buffer, "r");
	assert(fcsv != NULL);
	fclose(fcsv);

	//test if csv is of the desired length
	struct stat sb;
	if (stat(buffer, &sb) == -1) {
	    perror("stat");
	} else {
		printf("File size: %lld bytes\n", (long long) sb.st_size);
		assert(sb.st_size == 3066);
	}
}

CuSuite* CuPlotProducerSuite() {
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, testPlots2DProducer01);
	SUITE_ADD_TEST(suite, testPlot2DProducer02);
	SUITE_ADD_TEST(suite, testPlot2DProducer03);
	SUITE_ADD_TEST(suite, testPlot2DProducer04);
	SUITE_ADD_TEST(suite, testPlot2DProducer05);
	SUITE_ADD_TEST(suite, testPlot2DProducer06);
	SUITE_ADD_TEST(suite, testPlot2DProducer07);
	SUITE_ADD_TEST(suite, testPlot2DProducer08);
	SUITE_ADD_TEST(suite, testPlot2DProducer09);
	SUITE_ADD_TEST(suite, testPlot2DProducer10);
	SUITE_ADD_TEST(suite, testPlot2DProducer11);

	return suite;
}
