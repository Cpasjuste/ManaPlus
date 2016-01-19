/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2016  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "catch.hpp"
#include "client.h"
#include "logger.h"

#include "utils/physfstools.h"
#include "utils/xml.h"

#include "resources/resourcemanager.h"

#include "debug.h"

TEST_CASE("xml doc")
{
    client = new Client;
    PHYSFS_init("manaplus");
    dirSeparator = "/";
    XML::initXML();
    logger = new Logger();
    ResourceManager::init();
    resourceManager->addToSearchPath("data", Append_false);
    resourceManager->addToSearchPath("../data", Append_false);

    SECTION("load1")
    {
        XML::Document doc("graphics/gui/browserbox.xml",
            UseResman_true,
            SkipError_false);
        REQUIRE(doc.isLoaded() == true);
        REQUIRE(doc.isValid() == true);
        REQUIRE(doc.rootNode() != nullptr);
        REQUIRE(xmlNameEqual(doc.rootNode(), "skinset") == true);
    }

    SECTION("load2")
    {
        const std::string xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
            "<root><data option1=\"false\" option2=\"true\"/></root>";
        XML::Document doc(xml.c_str(), xml.size());
        REQUIRE(doc.isLoaded() == true);
        REQUIRE(doc.isValid() == true);
        REQUIRE(doc.rootNode() != nullptr);
        REQUIRE(xmlNameEqual(doc.rootNode(), "root") == true);
    }

    SECTION("load3")
    {
        const std::string xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
            "<!-- comment here\nand here -->"
            "<root><data option1=\"false\" option2=\"true\"/></root>";
        XML::Document doc(xml.c_str(), xml.size());
        REQUIRE(doc.isLoaded() == true);
        REQUIRE(doc.isValid() == true);
        REQUIRE(doc.rootNode() != nullptr);
        REQUIRE(xmlNameEqual(doc.rootNode(), "root") == true);
    }

    SECTION("properties")
    {
        XML::Document doc("graphics/gui/browserbox.xml",
            UseResman_true,
            SkipError_false);

        const XmlNodePtr rootNode = doc.rootNode();
        REQUIRE(XML::getProperty(rootNode, "image", "") == "window.png");
    }

    SECTION("for each")
    {
        XML::Document doc("graphics/gui/browserbox.xml",
            UseResman_true,
            SkipError_false);

        const XmlNodePtr rootNode = doc.rootNode();
        XmlNodePtr node = nullptr;
        for_each_xml_child_node(widgetNode, rootNode)
        {
            node = widgetNode;
            if (xmlNameEqual(node, "widget"))
                break;
        }
        REQUIRE(node != nullptr);
        REQUIRE(xmlNameEqual(node, "widget") == true);
        for_each_xml_child_node(optionNode, node)
        {
            node = optionNode;
            if (xmlNameEqual(node, "option"))
                break;
        }
        REQUIRE(node != nullptr);
        REQUIRE(xmlNameEqual(node, "option") == true);
        REQUIRE(XML::getProperty(node, "name", "") == "padding");
        REQUIRE(XML::langProperty(node, "name", "") == "padding");
        REQUIRE(XML::getProperty(node, "value", 0) == 1);
        REQUIRE(XML::getBoolProperty(node, "value", true) == true);
        REQUIRE(XML::getBoolProperty(node, "value", false) == false);
        REQUIRE(XML::getIntProperty(node, "value", -1, -10, 100) == 1);
    }

    SECTION("child1")
    {
        XML::Document doc("graphics/gui/browserbox.xml",
            UseResman_true,
            SkipError_false);

        const XmlNodePtr rootNode = doc.rootNode();
        XmlNodePtr node = XML::findFirstChildByName(rootNode, "widget");
        REQUIRE(node != nullptr);
        REQUIRE(xmlNameEqual(node, "widget") == true);
        node = XML::findFirstChildByName(node, "option");
        REQUIRE(node != nullptr);
        REQUIRE(xmlNameEqual(node, "option") == true);
        REQUIRE(XML::getProperty(node, "name", "") == "padding");
        REQUIRE(XML::langProperty(node, "name", "") == "padding");
        REQUIRE(XML::langProperty(node, "name123", "") == "");
        REQUIRE(XML::getProperty(node, "value", 0) == 1);
        REQUIRE(XML::getProperty(node, "value123", -1) == -1);
        REQUIRE(XML::getBoolProperty(node, "value", true) == true);
        REQUIRE(XML::getBoolProperty(node, "value", false) == false);
        REQUIRE(XML::getBoolProperty(node, "value123", true) == true);
        REQUIRE(XML::getIntProperty(node, "value", -1, -10, 100) == 1);
        REQUIRE(XML::getIntProperty(node, "value123", -1, -10, 100) == -1);
    }

    SECTION("child2")
    {
        const std::string xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
            "<root><data option1=\"false\" option2=\"true\" "
            "option3=\"10.5\"/></root>";
        XML::Document doc(xml.c_str(), xml.size());
        const XmlNodePtr rootNode = doc.rootNode();
        XmlNodePtr node = XML::findFirstChildByName(rootNode, "data");
        REQUIRE(node != nullptr);
        REQUIRE(xmlNameEqual(node, "data") == true);
        REQUIRE(XML::getBoolProperty(node, "option1", true) == false);
        REQUIRE(XML::getBoolProperty(node, "option2", false) == true);
        const float opt3 = XML::getFloatProperty(node, "option3", 0.0);
        REQUIRE(opt3 > 10);
        REQUIRE(opt3 < 11);
    }

    SECTION("child3")
    {
        const std::string xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
            "<!-- comment --><root><!-- comment -->"
            "<data option1=\"false\" option2=\"true\" "
            "option3=\"10.5\"/><!-- comment --></root>";
        XML::Document doc(xml.c_str(), xml.size());
        const XmlNodePtr rootNode = doc.rootNode();
        XmlNodePtr node = XML::findFirstChildByName(rootNode, "data");
        REQUIRE(node != nullptr);
        REQUIRE(xmlNameEqual(node, "data") == true);
        REQUIRE(XML::getBoolProperty(node, "option1", true) == false);
        REQUIRE(XML::getBoolProperty(node, "option2", false) == true);
        const float opt3 = XML::getFloatProperty(node, "option3", 0.0);
        REQUIRE(opt3 > 10);
        REQUIRE(opt3 < 11);
    }

    SECTION("validate")
    {
//        REQUIRE(XML::Document::validateXml(
//            "graphics/gui/browserbox.xml") == true);
        REQUIRE(XML::Document::validateXml(
            "graphics/gui/bubble.png") == false);
        REQUIRE(XML::Document::validateXml(
            "graphics/gui/testfile123.xml") == false);
    }
}