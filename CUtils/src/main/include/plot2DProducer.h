/**
 * \file plotProducer.h
 *
 * Represents a module allowing you to create plot graphs.
 *
 * The module is just a easy front end for <b>gnuplot</b> plot utilities.
 * In order to use this module, you need to have <b>gnuplot</b> installed on your system.
 *
 * <h1>Use</h1>
 *
 * The most simple example to use the module is the following one:
 *
 * @code
 * 	//initialize
 * 	Plot2DHelper* helper = initPlot2DHelper("output");
 * 	//add some points
 * 	addPointInSeries(helper, 1, 1);
 * 	addPointInSeries(helper, 2, 4);
 * 	addPointInSeries(helper, 3, 9);
 * 	addPointInSeries(helper, 4, 16);
 * 	//actually generate the plot
 * 	plotWith2DHelper(helper);
 * 	//free the memory
 * 	destroyPlot2DHelper(helper);
 * @endcode
 *
 *  Created on: Jan 16, 2017
 *      Author: koldar
 */

#ifndef PLOT2DPRODUCER_H_
#define PLOT2DPRODUCER_H_

#include <stdio.h>
#include <stdbool.h>
#include "list.h"

typedef enum PlotStyle {
	PS_LINES,
	PS_POINTS,
	PS_LINESPOINTS,
	PS_IMPULSES,
	PS_DOTS,
	PS_STEPS,
	PS_YERRORBARS,
	PS_XERRORBARS,
	PS_XYERRORBARS,
	PS_BOXES,
	PS_BOXERRORBARS,
	PS_BOXXYERRORBARS,
} PlotStyle;

/**
 * Detemrine how we plot an axis
 */
typedef enum AxisType {
	///default one. The axis is perfectly normal
	AT_STANDARD,
	///the axis is using a logaritmic step
	AT_LOGARITMIC
} AxisType;

typedef struct Series {
	const char* name;
	PlotStyle style;
} Series;

typedef list SeriesList;

/**
 * The main structure used to store support data for this module
 */
typedef struct Plot2DHelper {
	///the name where the output will be created. No extension
	const char* fileName;
	///a file containing the data gnuplot will use
	FILE* datFile;

	///a list containing all the series of the graph
	SeriesList* series;
	/**
	 * list of points previously added by ::addPoint
	 *
	 * If you're usig multiple series but you want to add a single series per time,
	 * the API needs to memorize somewhere the point previously added. This is the memory storage.
	 *
	 * @code
	 * addSeries(helper, "sin", PS_LINES);
	 * addSeries(helper, "cos", PS_LINES);
	 * addSeries(helper, "tan", PS_LINES);
	 *
	 * addPoint(helper, -1, sin(-1));
	 * addPoint(helper, -1, cos(-1));
	 * addPoint(helper, -1, tan(-1));
	 * addPoint(helper, 0, sin(0));
	 * addPoint(helper, 0, cos(0));
	 * addPoint(helper, 0, tan(0));
	 * addPoint(helper, +1, sin(+1));
	 * addPoint(helper, +1, cos(+1));
	 * addPoint(helper, +1, tan(+1));
	 *
	 * plotWith2DHelper(helper);
	 * @endcode
	 *
	 * \note
	 * This field will be initialized only if the series number is strictly greater than 1: if not, allocate new space would be a totally useless
	 * operation
	 */
	double* temporaryValues;
	/**
	 * the series we're analylzing with ::addPoint
	 *
	 * If ::Plot2DHelper::temporaryValues is  used, we also need to store the series the user is adding points into.
	 * In the first call of ::addPoint the first series is considered, then in the second series we need to use the second one and so on.
	 * The field contain such information.
	 */
	int currentSeries;

	/**
	 * Determine which character should be treated as comment line inside the input file
	 */
	const char* commentCharacters;
	/**
	 * Determine whether or not we should print a grid on the generated image
	 */
	bool enableGrid;
	/**
	 * Determine how we plot the x axis
	 */
	AxisType xAxisType;
	/**
	 * Determine how we plot the y axis
	 */
	AxisType yAxisType;
	/**
	 * The X label
	 */
	const char* xLabel;
	/**
	 * the Y label
	 */
	const char* yLabel;
	/**
	 * the title of the plot
	 */
	const char* title;
	/**
	 * The (optional) subtitle of the graph
	 *
	 * this field can be NULL
	 */
	const char* subtitle;
	/**
	 * Determine how the subtitle will be rendered
	 *
	 * True if you want to write the subtitle with a smaller font; False if you want to use the same
	 * font of the title but you want to wrap subtitle inside parenthesis
	 */
	bool reduceSubtitleFont;
	/**
	 * If you when we create the actual png plot we will create a CSV containing all the data plotted.
	 */
	bool createDataCSVFile;
} Plot2DHelper;

/**
 * Creates a support structure for printing plots
 *
 * @param[in] output name of output plot you want to have. No extension needed
 */
Plot2DHelper* initPlot2DHelper(const char* output);

/**
 * set the grid for the plot
 *
 * @param[inout] helper the support structure involved
 * @param[in] grid true if you want to display the grid, false otherwise
 */
void setGrid(Plot2DHelper* helper, bool grid);

/**
 * Allows you to create a csv containing all the data plotted inside the graph
 *
 * \note
 * This function is useful if you have plots whose data is the output of some sort of heavy computation.
 * Usually this is not the case though.
 *
 * @param[inout] helper the helper involved
 * @param[in] create true if you want to generate a csv file containing the plotted data before the actual plotting;
 */
void setCreateDataCSVFileOfPlot2DProducer(Plot2DHelper* helper, bool create);

/**
 * set label for x axis
 *
 * @param[inout] helper the support sturcture involved
 * @param[in] label the label to set
 */
void setXLabel(Plot2DHelper* helper, const char* label);
/**
 * set label for y axis
 *
 * @param[inout] helper the support sturcture involved
 * @param[in] label the label to set
 */
void setYLabel(Plot2DHelper* helper, const char* label);
/**
 * set title for the plot
 *
 * \attention
 * the title is set by \b reference: no copies are made!
 *
 * @param[inout] helper the support sturcture involved
 * @param[in] title the title of the plot to set
 */
void setTitle(Plot2DHelper* helper, const char* title);

/**
 * set the subtitle for the given plot
 *
 * \attention
 * the subtitle is set by \b reference: no copies are made!
 *
 * @param[inout] helper the support sturcture involved
 * @param[in] subtitle the subtitle of the plot to set
 */
void setSubtitle(Plot2DHelper* helper, const char* subtitle);

/**
 * Set the type of the x axis
 *
 * @param[inout] helper the structure to alter
 * @param[in] type the new type
 */
void setXAxisType(Plot2DHelper* helper, AxisType type);

/**
 * Set the type of the y axis
 *
 * @param[inout] helper the structure to alter
 * @param[in] type the new type
 */
void setYAxisType(Plot2DHelper* helper, AxisType type);

/**
 * Add a new plotline to draw inside the plot
 *
 * \note
 * This should be called before adding any points in the graph!
 *
 * @param[inout] helper the helper to use
 * @param[in] name the name of the series
 * @param[in] ps how should we render the plot line
 */
void addSeries(Plot2DHelper* helper, const char* name, const PlotStyle ps);

/**
 * Add a single point inside the the plot
 *
 * This function **heavily** depends on the order you use to call ::addSeries ! Assuming the followig:
 * @code
 * 	addSeries(helper, "foo", PS_LINES);
 * 	addSeries(helper, "bar", PS_LINES);
 *
 * 	addPoint(helper, 0, 3); addPoint(helper, 0, 2);
 * 	addPoint(helper, 0, 6); addPoint(helper, 0, 7);
 * @endcode
 *
 * point \f$<0,3>\f$ will be added in "foo" series while \f$<0,2>\f$ to "bar". Then \f$<0,6>\f$ will be the second point belonging to "foo" and
 * \f$<0,7>\f$ will set to "bar".
 *
 * @param[inout] helper the helper to use
 * @param[in] x the x of the point
 * @param[in] y the y of the point
 */
void addPoint(Plot2DHelper* helper, double x, double y);

/**
 * Like ::addPoint but you assign in one sweep all the series values for a particular "x" value. Assuming
 * you have 6 series \c ys will be an array of 6 elements, one per plot.
 *
 * Doing:
 * @code
 * addPoints(helper, 0, (double[]){1,2,3,4});
 * @encode
 * It's identical as doing:
 * @code
 * addPoint(helper, 0, 1);
 * addPoint(helper, 0, 2);
 * addPoint(helper, 0, 3);
 * addPoint(helper, 0, 4);
 * @endcode
 *
 * @param[inout] helper the helper to use
 * @param[in] x the x value;
 * @param[in] ys an array of values containing as many doubles as there are series in the \c helper
 */
void addPoints(Plot2DHelper* helper, double x, double* ys);

/**
 * Generates the image of the plot
 *
 * \attention
 * After calling this function, it won't be possible to add points anymore. Doing so will create
 * <b>undefined behaviours</b>
 *
 * @param[inout] helper the helper structure involved
 */
void plotWith2DHelper(Plot2DHelper* helper);

/**
 * Generates the image of the plot
 *
 * \attention
 * After calling this function, it won't be possible to add points anymore. Doing so will create
 * <b>undefined behaviours</b>
 *
 * @param[inout] helper the helper structure involved
 * @param[in] drawAlsoLogarithmScale if true, the function will generate 2 plots: the seocnd one will have the y-scale with log-10 step
 */
void plotsWith2DHelper(Plot2DHelper* helper, bool drawAlsoLogarithmScale);
/**
 * destroy the structure created by ::initPlot2DHelper
 *
 * \attention
 * The function won't remove the *.m nor *.png created by ::initPlot2DHelper
 *
 * @param[inout] plotHelper the structure to free from the memory
 */
void destroyPlot2DHelper(Plot2DHelper* plotHelper);



#endif /* PLOT2DPRODUCER_H_ */
