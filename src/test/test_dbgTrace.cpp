/*
 * test_dbgTrace.cpp
 *
 *  Created on: 22.04.2015
 *      Author: aschoepfer
 */

#include <DbgCliTopic.h>
#include <DbgPrintConsole.h>
#include <DbgTraceContext.h>
#include <DbgTraceLevel.h>
#include <DbgTraceOut.h>
#include <DbgTracePort.h>
#include <gtest/gtest.h>


TEST(dbgTrace, TraceContext)
{
  // Create CLI Node
  DbgCli_Node::AssignRootNode(new DbgCli_Topic(0, "trace", "Test Debug Trace Node."));
  EXPECT_NE((void*)0, DbgCli_Node::RootNode());
  EXPECT_EQ("trace", DbgCli_Node::RootNode()->getNodeName());
  EXPECT_EQ("Test Debug Trace Node.", DbgCli_Node::RootNode()->getHelpText());

  // Create debug trace context
  new DbgTrace_Context(static_cast<DbgCli_Topic*>(DbgCli_Node::RootNode()));
  EXPECT_NE((void*)0, DbgTrace_Context::getContext());
}

TEST(dbgTrace, TraceOut)
{
  const char* outName = "console";
  DbgTrace_Out* consoleOut = new DbgTrace_Out(DbgTrace_Context::getContext(), outName, new DbgPrint_Console());
  EXPECT_NE((void*)0, consoleOut);
  EXPECT_EQ("console", consoleOut->getName());
  DbgTrace_Out* testOut = DbgTrace_Context::getContext()->getTraceOut(outName);
  EXPECT_NE((void*)0, testOut);
}

TEST(dbgTrace, TracePort)
{
  const char* portTag = "TestPort";
  DbgTrace_Out* consoleOut = DbgTrace_Context::getContext()->getTraceOut("console");
  EXPECT_NE((void*)0, consoleOut);
  DbgTrace_Port* port = new DbgTrace_Port(portTag, consoleOut->getName(), DbgTrace_Level::notice);
  EXPECT_NE((void*)0, port);
  EXPECT_EQ("TestPort", port->getTag());
  EXPECT_EQ(DbgTrace_Level::notice, port->getLevel());
  DbgTrace_Port* testPort = DbgTrace_Context::getContext()->getTracePort(portTag);
  EXPECT_NE((void*)0, testPort);
}

TEST(dbgTrace, PrintTrace)
{
  DbgTrace_Port* port = DbgTrace_Context::getContext()->getTracePort("TestPort");
  EXPECT_NE((void*)0, port);
  TR_PRINT_STR(port, DbgTrace_Level::notice, "Hello Test")
  TR_PRINT_STR(port, DbgTrace_Level::notice, "9")
  TR_PRINT_STR(port, DbgTrace_Level::notice, "!рг?`+()\\/*-")
  TR_PRINT_STR(port, DbgTrace_Level::notice, "")
  TR_PRINT_LONG(port, DbgTrace_Level::notice, 9L)
  TR_PRINT_LONG(port, DbgTrace_Level::notice, 0.0)
  TR_PRINT_LONG(port, DbgTrace_Level::notice, 999)
  TR_PRINT_LONG(port, DbgTrace_Level::notice, 999999999)
  TR_PRINT_LONG(port, DbgTrace_Level::notice, -999999999)
  TR_PRINT_DBL(port, DbgTrace_Level::notice, 0.999999)
  TR_PRINT_DBL(port, DbgTrace_Level::notice, 0.9f)
  TR_PRINT_DBL(port, DbgTrace_Level::notice, -999.999999)
}
