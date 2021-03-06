#include <catch.hpp>
#include <Rule.hpp>

using namespace usbguard;

TEST_CASE("Non-printable characters in a rule string", "[RuleParser]") {
  Rule rule, rule_from;
  std::string rule_string;
  
  const uint8_t non_printable_cstr[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0xAA, 0xBB, 0xFF };
  const size_t non_printable_size = sizeof non_printable_cstr;
  const std::string non_printable_string((const char *)non_printable_cstr, non_printable_size);

  SECTION("to/from string: allow serial \"<non printable>\"") {
    rule.setTarget(Rule::Target::Allow);
    rule.setSerialNumber(non_printable_string);
    
    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(rule_string == "allow serial \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\"");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(rule_string == "allow serial \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\"");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }

  SECTION("to/from string: allow name \"<non printable>\"") {
    rule.setTarget(Rule::Target::Allow);
    rule.setDeviceName(non_printable_string);

    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(rule_string == "allow name \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\"");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(rule_string == "allow name \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\"");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }

  SECTION("to/from string: allow hash \"<non printable>\"") {
    rule.setTarget(Rule::Target::Allow);
    rule.setDeviceHash(non_printable_string);

    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(rule_string == "allow hash \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\"");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(rule_string == "allow hash \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\"");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }

  SECTION("to/from string: allow via-port \"<non printable>\"") {
    const std::vector<String> one_non_printable_string = { non_printable_string };
    rule.setTarget(Rule::Target::Allow);
    rule.setDevicePorts(one_non_printable_string);
    rule.setDevicePortsSetOperator(Rule::SetOperator::Equals);

    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(rule_string == "allow via-port \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\"");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(rule_string == "allow via-port \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\"");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }

  SECTION("to/from string: allow via-port { \"<non printable>\" \"<non printable>\" }") {
    const std::vector<String> two_non_printable_strings = \
      { non_printable_string, non_printable_string };
    rule.setTarget(Rule::Target::Allow);
    rule.setDevicePorts(two_non_printable_strings);
    rule.setDevicePortsSetOperator(Rule::SetOperator::OneOf);

    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(rule_string == "allow via-port one-of { \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\" \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\" }");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(rule_string == "allow via-port one-of { \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\" \"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\xaa\\xbb\\xff\" }");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }
}

TEST_CASE("Double quote and backslash characters in a rule string", "[RuleParser]") {
  Rule rule, rule_from;
  std::string rule_string;

  const std::string dqb_string("\" \\ \"\" \\\\ \\\" \"\\");
  const std::string dqb_string_escaped("\\\" \\\\ \\\"\\\" \\\\\\\\ \\\\\\\" \\\"\\\\");

  SECTION("to/from string: allow serial \"<double quote and backslash>\"") {
    rule.setTarget(Rule::Target::Allow);
    rule.setSerialNumber(dqb_string);

    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(rule_string == "allow serial \"" + dqb_string_escaped + "\"");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(rule_string == "allow serial \"" + dqb_string_escaped + "\"");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }

  SECTION("to/from string: allow name \"<double quote and backslash>\"") {
    rule.setTarget(Rule::Target::Allow);
    rule.setDeviceName(dqb_string);

    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(rule_string == "allow name \"" + dqb_string_escaped + "\"");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(rule_string == "allow name \"" + dqb_string_escaped + "\"");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }

  SECTION("to/from string: allow hash \"<double quote and backslash>\"") {
    rule.setTarget(Rule::Target::Allow);
    rule.setDeviceHash(dqb_string);

    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(rule_string == "allow hash \"" + dqb_string_escaped + "\"");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(rule_string == "allow hash \"" + dqb_string_escaped + "\"");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }

  SECTION("to/from string: allow via-port \"<double quote and backslash>\"") {
    const std::vector<String> one_dqb_string = { dqb_string };
    rule.setTarget(Rule::Target::Allow);
    rule.setDevicePorts(one_dqb_string);
    rule.setDevicePortsSetOperator(Rule::SetOperator::Equals);

    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(rule_string == "allow via-port \"" + dqb_string_escaped + "\"");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(rule_string == "allow via-port \"" + dqb_string_escaped + "\"");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }

  SECTION("to/from string: allow via-port { \"<double quote and backslash>\" \"<double quote and backslash>\" }") {
    const std::vector<String> two_dqb_strings = { dqb_string, dqb_string };
    rule.setTarget(Rule::Target::Allow);
    rule.setDevicePorts(two_dqb_strings);
    rule.setDevicePortsSetOperator(Rule::SetOperator::OneOf);

    REQUIRE_NOTHROW(rule_string = rule.toString());
    REQUIRE(rule_string == "allow via-port one-of { \"" + dqb_string_escaped + "\" \"" + dqb_string_escaped + "\" }");
    REQUIRE_NOTHROW(rule_from = Rule::fromString(rule_string));
    REQUIRE_NOTHROW(rule_string = rule_from.toString());
    REQUIRE(rule_string == "allow via-port one-of { \"" + dqb_string_escaped + "\" \"" + dqb_string_escaped + "\" }");
    REQUIRE(rule.appliesTo(rule_from));
    REQUIRE(rule_from.appliesTo(rule));
    REQUIRE(rule_from.getTarget() == Rule::Target::Allow);
  }
}
