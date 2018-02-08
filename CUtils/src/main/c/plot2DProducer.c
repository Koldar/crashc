/*
 * plot2DProducer.c
 *
 *  Created on: Jan 16, 2017
 *      Author: koldar
 */

#include "plot2DProducer.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "utility.h"
#include "log.h"
#include "list.h"
#include "macros.h"

static const char* plotStyleToString[] = {
		"lines",
		"points",
		"linespoints",
		"impulses",
		"dots",
		"steps",
		"yerrorbars",
		"xerrorbars",
		"xyerrorbars",
		"boxes",
		"boxerrorbars",
		"boxxyerrorbars"
};


static void generateGnuPlotScript(Plot2DHelper* helper, const char* suffixName);
static Series* initSeries(const char* name, PlotStyle ps);
static void destroySeries(Series* s);
static const char* getStringOfPlotStyle(const PlotStyle ps);
static double* generateTemporaryValues(const int seriesNumber);
static void processAxisType(FILE* f, AxisType at, char axis);
static void processTitleAndSubtitle(FILE* f, const Plot2DHelper* helper);
static void generateDataCSV(const Plot2DHelper* helper, const char* format, ...);

Plot2DHelper* initPlot2DHelper(const char* output)  {
	Plot2DHelper* retVal = malloc(sizeof(Plot2DHelper));
	if (retVal == NULL) {
		ERROR_MALLOC();
	}

	char buffer[BUFFER_SIZE];
	retVal->fileName = strdup(output);

	sprintf(buffer, "%s.dat", retVal->fileName);
	retVal->datFile = fopen(buffer, "w");
	if (retVal->datFile == NULL) {
		ERROR_FILE(retVal->fileName);
	}

	retVal->series = initList();
	retVal->temporaryValues = NULL;
	retVal->currentSeries = 0;

	retVal->enableGrid = false;
	retVal->commentCharacters = NULL;
	retVal->xLabel = NULL;
	retVal->yLabel = NULL;
	retVal->xAxisType = AT_STANDARD;
	retVal->yAxisType = AT_STANDARD;
	retVal->title = NULL;
	retVal->subtitle = NULL;
	retVal->reduceSubtitleFont = false;
	retVal->createDataCSVFile = false;

	return retVal;
}

void addPoint(Plot2DHelper* helper, double x, double y) {
	if (helper->series->size == 1) {
		return addPoints(helper, x, (double[]){y});
	}

	if ((helper->series->size > 1) && (helper->temporaryValues == NULL)) {
		helper->temporaryValues = generateTemporaryValues(helper->series->size);
	}

	helper->temporaryValues[helper->currentSeries] = y;
	helper->currentSeries++;
	if (helper->currentSeries == helper->series->size) {
		addPoints(helper, x, helper->temporaryValues);
		helper->currentSeries = 0;
	}
}

void addPoints(Plot2DHelper* helper, double x, double* ys) {
	fprintf(helper->datFile, "%.3f ", x);

	for (int i=0; i<helper->series->size; i++) {
		fprintf(helper->datFile, "%.3f", ys[i]);
		if ((i+1) < helper->series->size) {
			fprintf(helper->datFile, " ");
		}
	}
	fprintf(helper->datFile, "\n");
}

void setXAxisType(Plot2DHelper* helper, AxisType type) {
	helper->xAxisType = type;
}

void setYAxisType(Plot2DHelper* helper, AxisType type) {
	helper->yAxisType = type;
}

void addSeries(Plot2DHelper* helper, const char* name, const PlotStyle ps) {
	Series* s = initSeries(name, ps);

	addTailInList(helper->series, s);
}

void plotWith2DHelper(Plot2DHelper* helper) {
	plotsWith2DHelper(helper, false);
}

void plotsWith2DHelper(Plot2DHelper* helper, bool drawAlsoLogarithmScale) {
	fclose(helper->datFile);

	//create the csv file
	if (helper->createDataCSVFile) {
		generateDataCSV(helper, helper->fileName);
	}

	generateGnuPlotScript(helper, "");

	char buffer[BUFFER_SIZE];
	sprintf(buffer, "gnuplot %s.gp", helper->fileName);
	system(buffer);
	sprintf(buffer, "%s.gp", helper->fileName);
	remove(buffer);

	if (drawAlsoLogarithmScale) {
		setYAxisType(helper, AT_LOGARITMIC);
		generateGnuPlotScript(helper, "_log10");

		sprintf(buffer, "gnuplot %s.gp", helper->fileName);
		system(buffer);
		sprintf(buffer, "%s.gp", helper->fileName);
		remove(buffer);
	}
	sprintf(buffer, "%s.dat", helper->fileName);
	remove(buffer);
}

/**
 * Generates a CSV containing all the data inside the DAT file.
 * It's basically a conversion between dat file and csv file.
 *
 * @param[in] helper the helper to use
 * @param[in] format the string template to use as csv file name. Such template will then be appended with ".plotdata.csv" string
 * @param[in] ... the variadic arguments for \c format string
 */
static void generateDataCSV(const Plot2DHelper* helper, const char* format, ...) {
	char templateString[BUFFER_SIZE];
	char csvFilename[BUFFER_SIZE];
	va_list va;
	va_start(va, format);
	vsnprintf(templateString, BUFFER_SIZE, format, va);
	va_end(va);

	snprintf(csvFilename, BUFFER_SIZE, "%s.plotdata.csv", templateString);

	//we don't use csvhelper because we don't know how many columns we have. Or at least, the API of CsvHelper are not so easily flexible in this scenario!
	char datFileName[BUFFER_SIZE];
	snprintf(datFileName, BUFFER_SIZE, "%s.dat", helper->fileName);
	FILE* fin = fopen(datFileName, "r");
	if (fin == NULL) {
		ERROR_FILE(datFileName);
	}
	FILE* fout = fopen(csvFilename, "w");
	if (fout == NULL) {
		ERROR_FILE(csvFilename);
	}

	// *************** WRITE CSV HEADER ****************
	fprintf(fout, "sep=,\n");
	fprintf(fout, "X, ");
	int i = 0;
	ITERATE_ON_LIST(helper->series, cell, s, Series) {
		fprintf(fout, s->name);
		fprintf(fout, "%s", ((i+1) == helper->series->size) ? "\n" : ", ");
		i++;
	}

	// *************** WRITE CSV DATA ******************
	double value = -1;
	int elementsRead = 0;
	while(true) {
		//read x
		elementsRead = fscanf(fin, "%lf%*[ \n\t]", &value);
		if (elementsRead == EOF) {
			//reached the end of the input
			break;
		}
		if (elementsRead != 1) {
			ERROR_IMPOSSIBLE_SCENARIO("file dat is not valid! Read %d elements", elementsRead);
		}
		fprintf(fout, "%f, ", value);
		for (int i=0;  i<helper->series->size; i++) {
			//read y value
			if ((elementsRead = fscanf(fin, "%lf%*[ \n\t]", &value)) != 1) {
				ERROR_IMPOSSIBLE_SCENARIO("file dat is not valid! Read %d elements", elementsRead);
			}
			fprintf(fout, "%f", value);
			fprintf(fout, "%s", ((i+1) == helper->series->size) ? "\n" : ", ");
		}
	}

	fclose(fout);
}

/**
 * <a href="http://www.math.utk.edu/~vasili/refs/How-to/gnuplot.print.html">how to save gnuplot image</a>
 *
 * @param[in] helper the structure containing all information about the plotting
 * @param[in] a string appended at the end of output image
 */
static void generateGnuPlotScript(Plot2DHelper* helper, const char* suffixName) {
	char buffer[BUFFER_SIZE];
	sprintf(buffer,"%s.gp", helper->fileName);
	FILE* gnuPlotFile = fopen(buffer, "w");

	fprintf(gnuPlotFile, "reset\n");

	if (helper->commentCharacters != NULL) {
		fprintf(gnuPlotFile, "set datafile commentschars \"%s\"\n", helper->commentCharacters);
	}

	fprintf(gnuPlotFile, "set term png\n");
	fprintf(gnuPlotFile, "set output \"%s%s.png\"\n", helper->fileName, suffixName);

	processTitleAndSubtitle(gnuPlotFile, helper);

	if (helper->xLabel != NULL) {
		fprintf(gnuPlotFile, "set xlabel \"%s\"\n", helper->xLabel);
	}

	if (helper->yLabel != NULL) {
		fprintf(gnuPlotFile, "set ylabel \"%s\"\n", helper->yLabel);
	}

	processAxisType(gnuPlotFile, helper->xAxisType, 'x');
	fprintf(gnuPlotFile, "set xtic rotate\n");
	processAxisType(gnuPlotFile, helper->yAxisType, 'y');

	if (helper->enableGrid) {
		fprintf(gnuPlotFile, "set grid\n");
	}

	fprintf(gnuPlotFile, "plot ");

	int i = 0;
	ITERATE_ON_LIST(helper->series, cell, s, Series) {
		fprintf(gnuPlotFile, "\"%s.dat\" using 1:%d title \"%s\" with %s", helper->fileName, (i+2), s->name, getStringOfPlotStyle(s->style));
		if ((i+1) < helper->series->size) {
			fprintf(gnuPlotFile, ", ");
		} else {
			fprintf(gnuPlotFile, "\n");
		}
		i++;
	}

	fclose(gnuPlotFile);
}

void setGrid(Plot2DHelper* helper, bool grid) {
	helper->enableGrid = grid;
}

void setCreateDataCSVFileOfPlot2DProducer(Plot2DHelper* helper, bool create) {
	helper->createDataCSVFile = create;
}

void setCommentStartCharacters(Plot2DHelper* helper, const char* strings) {
	helper->commentCharacters = strings;
}

void setXLabel(Plot2DHelper* helper, const char* label) {
	helper->xLabel = label;
}

void setYLabel(Plot2DHelper* helper, const char* label) {
	helper->yLabel = label;
}

void setTitle(Plot2DHelper* helper, const char* title) {
	helper->title = title;
}

void setSubtitle(Plot2DHelper* helper, const char* subtitle) {
	helper->subtitle = subtitle;
}

void destroyPlot2DHelper(Plot2DHelper* plotHelper) {
	destroyListWithElement(plotHelper->series, destroySeries);
	if (plotHelper->temporaryValues != NULL) {
		free(plotHelper->temporaryValues);
	}
	free(plotHelper->fileName);
	free(plotHelper);
}

static Series* initSeries(const char* name, PlotStyle ps) {
	Series* retVal = malloc(sizeof(Series));
	if (retVal == NULL) {
		ERROR_MALLOC();
	}

	retVal->name = strdup(name);
	retVal->style = ps;

	return retVal;
}

static void destroySeries(Series* s) {
	free(s->name);
	free(s);
}

static const char* getStringOfPlotStyle(const PlotStyle ps) {
	return plotStyleToString[ps];
}

/**
 * Generates a perfectly empty array containing temporary values
 *
 * You need to perform this operation as soon as you know the exact number of series in the graph
 *
 * @param[in] seriesNumber the number of series in the graph
 * @return a pointer to the first cell of the array containing temporary numbers
 */
static double* generateTemporaryValues(const int seriesNumber) {
	double* retVal = calloc(seriesNumber, sizeof(double));
	if (retVal == NULL) {
		ERROR_MALLOC();
	}
	return retVal;
}

static void processAxisType(FILE* f, AxisType at, char axis) {
	switch (at) {
	case AT_STANDARD : {
		fprintf(f,"unset logscale %c\n", axis);
		break;
	}
	case AT_LOGARITMIC: {
		fprintf(f, "set logscale %c 10\n", axis);
		//see format specifier of gnu plot manual
		fprintf(f, "set format %c \"%%.1lE%%+3L\"\n", axis);
		break;
	}
	default: {
		ERROR_UNHANDLED_CASE("axis type", at);
	}
	}
}

/**
 * Print on \c f the title and the subtitle of the graph
 *
 * \note
 * The implementation is based upon the following <a href="http://stackoverflow.com/a/13645897/1887602">link</a>
 *
 * The function does \b nothing if ::Plot2DHelper::title is NULL
 *
 * @param[inout] f the file where we need to write on
 * @param[in] helper the data structure with the title/subtitle information
 */
static void processTitleAndSubtitle(FILE* f, const Plot2DHelper* helper) {

	//set title "Big Title\n{/*0.5 subtitle}"
	if (helper->title != NULL) {
		fprintf(f, "set title \"%s", helper->title);
		if (helper->subtitle != NULL) {
			if (helper->reduceSubtitleFont) {
				fprintf(f, "\\n{/*0.8 %s}", helper->subtitle);
			} else {
				fprintf(f, "\\n(%s)", helper->subtitle);
			}

		}
		fprintf(f, "\"\n");
	}
}
