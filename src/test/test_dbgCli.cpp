/*
 * test_dbgCli.cpp
 *
 *  Created on: 14.04.2015
 *      Author: niklausd
 */

#include "gtest/gtest.h"

#include "DbgCliTopic.h"
#include "DbgCliCommand.h"

TEST(dbgCli, RootNode)
{
  DbgCli_Node::AssignRootNode(new DbgCli_Topic(0, "dbg", "Test Debug CLI Root Node."));
  EXPECT_NE((void*)0, DbgCli_Node::RootNode());
}
