#include <stdio.h>
#include "CuTest.h"
#include "log.h"
#include <stdlib.h>

CuSuite* CuProjectSuite();
CuSuite* CuHTSuite();
CuSuite* CuGraphSuite();
CuSuite* CuStackSuite();
CuSuite* CuSimpleLoopComputerSuite();
CuSuite* CuStringBuilderSuite();
CuSuite* CuRegexSuite();
CuSuite* CuCSVSuite();
CuSuite* CuPlotProducerSuite();
CuSuite* CuTopologicalOrderSuite();
CuSuite* CuHeapSuite();
CuSuite* CuBinarySearchTreeSuite();
CuSuite* CuRedBlackTreeSuite();
CuSuite* CuSCCSuite();
CuSuite* CuLogSuite();
CuSuite* CuMacroSuite();
CuSuite* CuDynamicArraySuite();
CuSuite* CuQueueSuite();
CuSuite* CuArgParserSuite();
CuSuite* CuDynamicStackSuite();
CuSuite* CuSpaceMeasurementSuite();
CuSuite* CuHashSetSuite();



void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuite* listSuite = CuProjectSuite();
	CuSuite* htSuite = CuHTSuite();
	CuSuite* graphSuite = CuGraphSuite();
	CuSuite* stackSuite = CuStackSuite();
	CuSuite* stringBuilderSuite = CuStringBuilderSuite();
	CuSuite* regexSuite = CuRegexSuite();
	CuSuite* csvSuite = CuCSVSuite();
	CuSuite* plotProducerSuite = CuPlotProducerSuite();
	CuSuite* topologicalOrderSuite = CuTopologicalOrderSuite();
	CuSuite* heapSuite = CuHeapSuite();
	CuSuite* binarySearchTreeSuite = CuBinarySearchTreeSuite();
	CuSuite* redBlackTreeSuite = CuRedBlackTreeSuite();
	CuSuite* sccSuite = CuSCCSuite();
	CuSuite* logSuite = CuLogSuite();
	CuSuite* macroSuite = CuMacroSuite();
	CuSuite* dynamicArraySuite = CuDynamicArraySuite();
	CuSuite* simpleLoopComputer = CuSimpleLoopComputerSuite();
	CuSuite* queueSuite = CuQueueSuite();
	CuSuite* argParserSuite = CuArgParserSuite();
	CuSuite* dynamicStackSuite = CuDynamicStackSuite();
	CuSuite* spaceMeasurementSuite = CuSpaceMeasurementSuite();
	CuSuite* hashSetSuite = CuHashSetSuite();


	CuSuiteAddSuite(suite, listSuite);
	CuSuiteAddSuite(suite, htSuite);
	CuSuiteAddSuite(suite, graphSuite);
	CuSuiteAddSuite(suite, stackSuite);
	CuSuiteAddSuite(suite, stringBuilderSuite);
	CuSuiteAddSuite(suite, regexSuite);
	CuSuiteAddSuite(suite, csvSuite);
	CuSuiteAddSuite(suite, plotProducerSuite);
	CuSuiteAddSuite(suite, topologicalOrderSuite);
	CuSuiteAddSuite(suite, heapSuite);
	CuSuiteAddSuite(suite, binarySearchTreeSuite);
	CuSuiteAddSuite(suite, redBlackTreeSuite);
	CuSuiteAddSuite(suite, sccSuite);
	CuSuiteAddSuite(suite, logSuite);
	CuSuiteAddSuite(suite, macroSuite);
	CuSuiteAddSuite(suite, dynamicArraySuite);
	CuSuiteAddSuite(suite, simpleLoopComputer);
	CuSuiteAddSuite(suite, queueSuite);
	CuSuiteAddSuite(suite, dynamicStackSuite);
	CuSuiteAddSuite(suite, spaceMeasurementSuite);
	CuSuiteAddSuite(suite, hashSetSuite);
//	CuSuiteAddSuite(suite, argParserSuite);

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);

	free(listSuite);
	free(htSuite);
	free(graphSuite);
	free(stackSuite);
	free(stringBuilderSuite);
	free(regexSuite);
	free(csvSuite);
	free(plotProducerSuite);
	free(topologicalOrderSuite);
	free(heapSuite);
	free(binarySearchTreeSuite);
	free(redBlackTreeSuite);
	free(sccSuite);
	free(logSuite);
	free(macroSuite);
	free(dynamicArraySuite);
	free(simpleLoopComputer);
	free(queueSuite);
	free(argParserSuite);
	free(dynamicStackSuite);
	free(spaceMeasurementSuite);
	free(hashSetSuite);

	CuStringDelete(output);

	CuSuiteDelete(suite);
	clearExcludedLoggers();
}

int main(void)
{
	RunAllTests();
}
