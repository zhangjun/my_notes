#pragma once 

namespace csstd{
	/*define remove_reference overload*/
	template<typename _Tp>
	struct remove_reference
	{typedef _Tp type;};
	     
	template<typename _Tp> 
	struct remove_reference<_Tp&>
	{typedef _Tp type;};
		 
	template<typename _Tp> 
	struct remove_reference<_Tp&&>
	{typedef _Tp type;};
		        
	template<typename _Tp>
	constexpr typename csstd::remove_reference<_Tp>::type&&
	move(_Tp&& __t) noexcept
	{ return static_cast<typename csstd::remove_reference<_Tp>::type&&>(__t);}
}

