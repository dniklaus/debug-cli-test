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
  DbgCli_Node::AssignRootNode(new DbgCli_Topic(0, "mmi", "Test Debug CLI Root Node."));
  EXPECT_NE((void*)0, DbgCli_Node::RootNode());
  EXPECT_EQ("mmi", DbgCli_Node::RootNode()->getNodeName());
  EXPECT_EQ("Test Debug CLI Root Node.", DbgCli_Node::RootNode()->getHelpText());
}

/*
 * Test Tree
 *
 *            mmi (root)
 *            /
 *        screen-------sensorA   (sibling of screen)
 *          /              \
 *    show (command)      subX
 *                           \
 *                         topicY----commandZ
 */

TEST(dbgCli, AddTopic)
{
// MUST NOT be done again!! //  DbgCli_Node::AssignRootNode(new DbgCli_Topic(0, "mmi", "Test Debug CLI Root Node."));
  DbgCli_Topic* screenNode = new DbgCli_Topic(DbgCli_Node::RootNode(), "screen", "Screen Node.");
  EXPECT_EQ(screenNode, DbgCli_Node::RootNode()->getNode("mmi", "screen"));
  EXPECT_EQ("screen", DbgCli_Node::RootNode()->getNode("mmi", "screen")->getNodeName());
  EXPECT_EQ("Screen Node.", DbgCli_Node::RootNode()->getNode("mmi", "screen")->getHelpText());
}

TEST(dbgCli, AddCommand)
{
  DbgCli_Command* showCmd = new DbgCli_Command(DbgCli_Node::RootNode()->getNode("mmi", "screen"), "show", "Show Node, shows current screen.");
  EXPECT_EQ(showCmd, DbgCli_Node::RootNode()->getNode("mmi screen", "show"));
  EXPECT_EQ("show", DbgCli_Node::RootNode()->getNode("mmi screen", "show")->getNodeName());
  EXPECT_EQ("Show Node, shows current screen.", DbgCli_Node::RootNode()->getNode("mmi screen", "show")->getHelpText());
}

TEST(dbgCli, AddOtherBranch)
{
  DbgCli_Topic* sensorA = new DbgCli_Topic(DbgCli_Node::RootNode(), "sensorA", "This is Sensor A.");
  EXPECT_EQ(sensorA, DbgCli_Node::RootNode()->getNode("mmi", "sensorA"));
  EXPECT_EQ("sensorA", DbgCli_Node::RootNode()->getNode("mmi", "sensorA")->getNodeName());
  EXPECT_EQ("This is Sensor A.", DbgCli_Node::RootNode()->getNode("mmi", "sensorA")->getHelpText());

  DbgCli_Topic* subX = new DbgCli_Topic(sensorA, "subX", "This is a subsystem x of sensor A.");
  EXPECT_EQ(subX, DbgCli_Node::RootNode()->getNode("mmi sensorA", "subX"));
  EXPECT_EQ("subX", DbgCli_Node::RootNode()->getNode("mmi sensorA", "subX")->getNodeName());
  EXPECT_EQ("This is a subsystem x of sensor A.", DbgCli_Node::RootNode()->getNode("mmi sensorA", "subX")->getHelpText());

  DbgCli_Topic* topicY = new DbgCli_Topic(subX, "topicY", "TopicY in subsystemX of sensor A");
  EXPECT_EQ(topicY, DbgCli_Node::RootNode()->getNode("mmi sensorA subX", "topicY"));
  EXPECT_EQ("topicY", DbgCli_Node::RootNode()->getNode("mmi sensorA subX", "topicY")->getNodeName());
  EXPECT_EQ("TopicY in subsystemX of sensor A", DbgCli_Node::RootNode()->getNode("mmi sensorA subX", "topicY")->getHelpText());

  DbgCli_Command* commandZ = new DbgCli_Command(subX, "commandZ", "CommandZ in subsystemX of sensor A");
  EXPECT_EQ(commandZ, DbgCli_Node::RootNode()->getNode("mmi sensorA subX", "commandZ"));
  EXPECT_EQ("commandZ", DbgCli_Node::RootNode()->getNode("mmi sensorA subX", "commandZ")->getNodeName());
  EXPECT_EQ("CommandZ in subsystemX of sensor A", DbgCli_Node::RootNode()->getNode("mmi sensorA subX", "commandZ")->getHelpText());
}

TEST(dbgCli, AddTopicSiblingToCommand)
{
  new DbgCli_Topic(DbgCli_Node::RootNode()->getChildNode("screen"), "screensub", "Subsystem to Screen");
  EXPECT_EQ("screensub", DbgCli_Node::RootNode()->getNode("mmi screen", "screensub")->getNodeName());
  EXPECT_EQ("Subsystem to Screen", DbgCli_Node::RootNode()->getNode("mmi screen", "screensub")->getHelpText());
}

TEST(dbgCli, ExecuteNodes)
{
  const char* args_mmi_screen_show[] = {"mmi", "screen", "show"};
  unsigned int argc = 3;
  DbgCli_Topic::RootNode()->execute(argc, args_mmi_screen_show, 1);
}
