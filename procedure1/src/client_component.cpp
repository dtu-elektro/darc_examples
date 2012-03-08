#include <darc/component.h>
#include <darc/procedure/client.h>
#include <darc/timer/periodic_timer.h>

#include <std_msgs/Int32.h>

class MyClientComponent : public darc::Component
{
protected:
  darc::procedure::Client<std_msgs::Int32, std_msgs::Int32, std_msgs::Int32> procedure_client_;
  darc::timer::PeriodicTimer timer_;

protected:
  void timerHandler()
  {
    std_msgs::Int32::Ptr msg( new std_msgs::Int32 );
    msg->data = 10;
    procedure_client_.call(msg);
  }

  void statusHandler(const darc::procedure::CallID& call_id, boost::shared_ptr<std_msgs::Int32> msg)
  {
    DARC_INFO("Feedback Call %s", call_id.short_string().c_str());
  }

  void returnHandler(const darc::procedure::CallID& call_id, boost::shared_ptr<std_msgs::Int32> msg)
  {
    DARC_INFO("Result Call %s", call_id.short_string().c_str());
  }

public:
  MyClientComponent( const std::string& instance_name, darc::Node::Ptr node ) :
    darc::Component(instance_name, node),
    procedure_client_( this,
		       "myprocedure",
		       boost::bind( &MyClientComponent::returnHandler, this, _1, _2 ),
		       boost::bind( &MyClientComponent::statusHandler, this, _1, _2 ) ),
    timer_( this, boost::bind(&MyClientComponent::timerHandler, this), boost::posix_time::seconds(1) )
  {
  }

};

DARC_REGISTER_COMPONENT(MyClientComponent)
