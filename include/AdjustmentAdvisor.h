#ifndef AdjustmentAdvisor_h
#define AdjustmentAdvisor_h

#include "clocklab_types.h" 

//=============================================================================================================
class AdjustmentAdvisor
{
  private:
    src_type_t selectedSource;

  public:
    AdjustmentAdvisor( void);
    ~AdjustmentAdvisor() {};

    void setSelectedSource( src_type_t src);
    bool routeSource( MessageTime_t &bestSrc_msg, const MessageTime_t &new_msg);
};

//=============================================================================================================

#endif // AdjustmentAdvisor