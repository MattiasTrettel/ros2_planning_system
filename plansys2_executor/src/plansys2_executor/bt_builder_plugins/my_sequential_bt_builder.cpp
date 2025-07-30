#include "plansys2_executor/bt_builder_plugins/sequential_bt_builder.hpp"
#include "pluginlib/class_list_macros.hpp"

#include <sstream>
#include <string>

namespace plansys2
{

SequentialBTBuilder::SequentialBTBuilder()
{
}

void SequentialBTBuilder::initialize(
  const std::string & bt_action_1,
  const std::string & /*bt_action_2*/,
  int /*precision*/)
{
  if(!bt_action_1.empty()){
    bt_action_template_ = bt_action_1;
  }
  else{
    bt_action_template_ =
      R""""(<Sequence name="ACTION_ID">
WAIT_PREV_ACTIONS
  <ApplyAtStartEffect action="ACTION_ID"/>
  <ReactiveSequence name="ACTION_ID">
    <CheckOverAllReq action="ACTION_ID"/>
    <ExecuteAction action="ACTION_ID"/>
  </ReactiveSequence>
  <CheckAtEndReq action="ACTION_ID"/>
  <ApplyAtEndEffect action="ACTION_ID"/>
</Sequence>
)"""";
  }
}

std::string SequentialBTBuilder::get_tree(const plansys2_msgs::msg::Plan & plan){
    std::ostringstream tree;
    tree << "<root main_tree_to_execute=\"MainTree\">\n";
    tree << t(1) << "<BehaviorTree ID=\"MainTree\">\n";
    tree << t(2) << "<Sequence name=\"MainSequence\">\n";

    std::vector<std::string> previous_ids;

    for (const auto & item : plan.items) {
        const std::string action_id = "(" + item.action + "):" + std::to_string(static_cast<int>(item.time * 1000));
        std::string block = build_action_block(action_id, previous_ids, 3);
        tree << block;
        previous_ids.push_back(action_id);
    }

    tree << t(2) << "</Sequence>\n";
    tree << t(1) << "</BehaviorTree>\n";
    tree << "</root>\n";

    return tree.str();
}

std::string SequentialBTBuilder::build_action_block(const std::string & action_id, const std::vector<std::string> & dependencies, int indent){
    std::string ret;
    std::string block = bt_action_template_;

    std::string wait_str;
    for(const auto & dep : dependencies){
        wait_str += t(1) + "<WaitAction action=\"" + dep + "\"/>\n";
    }

    size_t pos;
    while((pos = block.find("ACTION_ID")) != std::string::npos){
        block.replace(pos, 9, action_id);
    }
    while((pos = block.find("WAIT_PREV_ACTIONS")) != std::string::npos){
        block.replace(pos, 18, wait_str);
    }

    std::istringstream lines(block);
    std::string line;
    while(std::getline(lines, line)){
        if(!line.empty()){
            ret += t(indent) + line + "\n";
        }
    }

  return ret;
}

std::string SequentialBTBuilder::t(int level){
    return std::string(level * 2, ' ');
}

}  // namespace plansys2

PLUGINLIB_EXPORT_CLASS(plansys2::SequentialBTBuilder, plansys2::BTBuilder)
