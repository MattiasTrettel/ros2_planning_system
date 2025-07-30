#ifndef PLANSYS2_EXECUTOR__BT_BUILDER_PLUGINS__SEQUENTIAL_BT_BUILDER_HPP_
#define PLANSYS2_EXECUTOR__BT_BUILDER_PLUGINS__SEQUENTIAL_BT_BUILDER_HPP_

#include <string>
#include <memory>
#include <map>

#include "plansys2_executor/BTBuilder.hpp"
#include "plansys2_executor/ActionExecutor.hpp"
#include "plansys2_msgs/msg/plan.hpp"

namespace plansys2
{

class SequentialBTBuilder : public BTBuilder{
    public:
        SequentialBTBuilder();
    
        void initialize(
            const std::string & bt_action_1 = "",
            const std::string & bt_action_2 = "",
            int precision = 3) override;
    
        std::string get_tree(const plansys2_msgs::msg::Plan & plan) override;
    
        std::string get_dotgraph(
            std::shared_ptr<std::map<std::string, ActionExecutionInfo>> /*action_map*/,
            bool /*enable_legend*/ = false,
            bool /*enable_print_graph*/ = false) override
        {
            return "";  // No graph for this simple builder
        }
  
    private:
        std::string build_action_block(const std::string & action_id, const std::vector<std::string> & dependencies, int indent);
        std::string t(int level);
        
        std::string bt_action_template_;
};

}  // namespace plansys2

#endif  // PLANSYS2_EXECUTOR__BT_BUILDER_PLUGINS__SEQUENTIAL_BT_BUILDER_HPP_
