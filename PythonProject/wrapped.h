#pragma once
#define BOOST_PYTHON_STATIC_LIB  
#include "boost/python/detail/wrap_python.hpp"
#include <boost/python.hpp>
#include "bakkesmod/wrappers/includes.h"
#include "bakkesmod/wrappers/arraywrapper.h"
using namespace boost::python;

//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(log_overloads, ConsoleWrapper::log, 1, 2);
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(set_angular_overloads, ActorWrapper::SetAngularVelocity, 1, 2);
void (CVarWrapper::*setString)(string) = &CVarWrapper::setValue;
void (CVarWrapper::*setInt)(int) = &CVarWrapper::setValue;
void (CVarWrapper::*setFloat)(float) = &CVarWrapper::setValue;

//class Helpers {};
struct unusedbutneededtocompile {
	unusedbutneededtocompile() {}
	ArrayWrapper<ActorWrapper> aaa = ArrayWrapper<ActorWrapper>(uintptr_t(NULL));
	ArrayWrapper<TeamWrapper> aab = ArrayWrapper<TeamWrapper>(uintptr_t(NULL));
	ArrayWrapper<CarWrapper> aac = ArrayWrapper<CarWrapper>(uintptr_t(NULL));
	ArrayWrapper<BallWrapper> aad = ArrayWrapper<BallWrapper>(uintptr_t(NULL));
	ArrayWrapper<PriWrapper> aae = ArrayWrapper<PriWrapper>(uintptr_t(NULL));
	ArrayWrapper<WheelWrapper> aaf = ArrayWrapper<WheelWrapper>(uintptr_t(NULL));
	ArrayWrapper<RBActorWrapper> aag = ArrayWrapper<RBActorWrapper>(uintptr_t(NULL));
	ArrayWrapper<CarComponentWrapper> aah = ArrayWrapper<CarComponentWrapper>(uintptr_t(NULL));
	ArrayWrapper<GoalWrapper> aai = ArrayWrapper<GoalWrapper>(uintptr_t(NULL));
};

#define PYTHON_ARRAY(WrappedArrayClass) \
	class_<ArrayWrapper<WrappedArrayClass>, boost::noncopyable>("ArrayWrapper#WrappedArrayClass", no_init).\
		def("Count", &ArrayWrapper<WrappedArrayClass>::Count).\
		def("Get", &ArrayWrapper<WrappedArrayClass>::Get);

BOOST_PYTHON_MODULE(bakkesmod)
{


	class_<BakPy>("BakPy", no_init).
		def("SetTimeout", &BakPy::set_timeout).
		def("VectorToRotator", VectorToRotator).staticmethod("VectorToRotator").
		def("RotatorToVector", RotatorToVector).staticmethod("RotatorToVector");;//.staticmethod("set_timeout");

	//class_<Helpers>("Helpers").
	//	def("get_safe_float_range", get_safe_float_range).staticmethod("get_safe_float_range").
	//	def("get_safe_float_range_exclude", get_safe_float_range_exclude).staticmethod("get_safe_float_range_exclude").
	//	def("get_safe_float", get_safe_float).staticmethod("get_safe_float").
	//	def("get_safe_int", get_safe_int).staticmethod("get_safe_int").
	//	def("file_exists", file_exists).staticmethod("file_exists").
	//	def("is_number", is_number).staticmethod("is_number");

	//class_<LogLevel>("LogLevel", no_init).
	//	def_readwrite("r", &LogLevel::r).
	//	def_readwrite("g", &LogLevel::g).
	//	def_readwrite("b", &LogLevel::b).
	//	def_readwrite("a", &LogLevel::a);
	class_<CVarWrapper>("CVarWrapper", no_init).
		def("GetCvarName", &CVarWrapper::getCVarName).
		def("GetIntValue", &CVarWrapper::getIntValue).
		def("GetFloatValue", &CVarWrapper::getFloatValue).
		def("GetBoolValue", &CVarWrapper::getBoolValue).
		def("GetStringValue", &CVarWrapper::getStringValue).
		def("GetDescription", &CVarWrapper::getDescription).
		def("Notify", &CVarWrapper::notify).
		def("SetValue", setString).
		def("SetValue", setInt).
		def("SetValue", setFloat);

	class_<CVarManagerWrapper>("CVarManagerWrapper", no_init).
		def("ExecuteCommand", &CVarManagerWrapper::executeCommand).
		//.def("register_notifier", CVarManagerWrapper::registerNotifier). //No need to give acces to this as t heyre C++ callbacks and python scripts should have standalone functions
		def("RegisterCvar", &CVarManagerWrapper::registerCvar).
		def("Log", &CVarManagerWrapper::log).
		def("GetCvar", &CVarManagerWrapper::getCvar);

	class_<GameWrapper, boost::noncopyable>("GameWrapper", no_init).
		def("IsInTutorial", &GameWrapper::IsInTutorial).
		def("IsInReplay", &GameWrapper::IsInReplay).
		def("IsInCustomTraining", &GameWrapper::IsInCustomTraining).
		//def("get_game_event", &GameWrapper::GetGameEvent).
		def("GetGameEventAsTutorial", &GameWrapper::GetGameEventAsTutorial).
		def("GetGameEventAsServer", &GameWrapper::GetGameEventAsServer).
		def("GetGameEventAsReplay", &GameWrapper::GetGameEventAsReplay).
		def("GetLocalCar", &GameWrapper::GetLocalCar).
		def("GetCamera", &GameWrapper::GetCamera).
		//def("set_timeout", &GameWrapper::SetTimeout). //time in MS
		def("Execute", &GameWrapper::Execute). //Use this when calling from a different thread
		def("RegisterDrawable", &GameWrapper::RegisterDrawable).
		def("GetFNameByIndex", &GameWrapper::GetFNameByIndex);
		//def("get_solid_hook", &GameWrapper::GetSolidHook).
		//def("get_calculator", &GameWrapper::GetCalculator);

	class_<PredictionInfo>("PredictionInfo").
		def(init<>()).
		def_readwrite("Time", &PredictionInfo::Time).
		def_readwrite("Location", &PredictionInfo::Location).
		def_readwrite("Velocity", &PredictionInfo::Velocity).
		def_readwrite("HitWall", &PredictionInfo::HitWall).
		def_readwrite("HitFloor", &PredictionInfo::HitFloor);

	class_<ControllerInput>("ControllerInput").
		def(init<>()).
		def_readwrite("throttle", &ControllerInput::Throttle).
		def_readwrite("steer", &ControllerInput::Steer).
		def_readwrite("pitch", &ControllerInput::Pitch).
		def_readwrite("yaw", &ControllerInput::Yaw).
		def_readwrite("roll", &ControllerInput::Roll).
		def_readwrite("dodge_forward", &ControllerInput::DodgeForward).
		def_readwrite("dodge_strafe", &ControllerInput::DodgeStrafe).
		def_readwrite("jump", &ControllerInput::Jump).
		def_readwrite("activate_boost", &ControllerInput::ActivateBoost).
		def_readwrite("holding_boost", &ControllerInput::HoldingBoost).
		def_readwrite("handbrake", &ControllerInput::Handbrake).
		def_readwrite("jumped", &ControllerInput::Jumped);

	class_<POV>("POV").
		def(init<>()).
		def_readwrite("location", &POV::location).
		def_readwrite("rotation", &POV::rotation).
		def_readwrite("fov", &POV::FOV);

	class_<Vector2>("Vector2").
		def(init<>()).
		def_readwrite("x", &Vector2::X).
		def_readwrite("y", &Vector2::Y);


	class_<Vector>("Vector").
		def(init<float, float, float>()).
		def(init<float>()).
		def(init<>()).
		def_readwrite("X", &Vector::X).
		def_readwrite("Y", &Vector::Y).
		def_readwrite("Z", &Vector::Z).
		def(self + Vector()).
		def(self - Vector()).
		def(self * Vector()).
		def(self / Vector()).
		def("Magnitude", &Vector::magnitude).
		def("Normalize", &Vector::normalize).
		def("Clone", &Vector::clone).
		def("Dot", &Vector::dot).
		def("Cross", &Vector::cross).
		def("Lerp", &Vector::lerp).
		def("Slerp", &Vector::slerp);


	class_<Rotator>("Rotator").
		def(init<float, float, float>()).
		def(init<float>()).
		def(init<>()).
		def_readwrite("pitch", &Rotator::Pitch).
		def_readwrite("yaw", &Rotator::Yaw).
		def_readwrite("roll", &Rotator::Roll).
		def(self + Rotator()).
		def(self - Rotator()).
		def(self * Rotator()).
		def(self / Rotator());


	class_<ObjectWrapper>("ObjectWrapper", no_init);

	class_<ActorWrapper, bases<ObjectWrapper>>("ActorWrapper", no_init).
		def("GetVelocity", &ActorWrapper::GetVelocity).
		def("SetVelocity", &ActorWrapper::SetVelocity).
		def("GetLocation", &ActorWrapper::GetLocation).
		def("SetLocation", &ActorWrapper::SetLocation).
		def("GetVelocity", &ActorWrapper::GetVelocity).
		def("SetVelocity", &ActorWrapper::SetVelocity).
		def("AddVelocity", &ActorWrapper::AddVelocity).
		def("GetRotation", &ActorWrapper::GetRotation).
		def("SetRotation", &ActorWrapper::SetRotation).
		def("SetTorque", &ActorWrapper::SetTorque).
		def("Stop", &ActorWrapper::Stop).
		//def("freeze", &ActorWrapper::Freeze).
		//def("is_hooked", &ActorWrapper::IsHooked).
		def("GetAngularVelocity", &ActorWrapper::GetAngularVelocity).
		def("SetAngularVelocity", &ActorWrapper::SetAngularVelocity).
		def("IsNull", &ActorWrapper::IsNull).
		def("GetDrawScale", &ActorWrapper::GetDrawScale).
		def("SetDrawScale", &ActorWrapper::SetDrawScale).
		def("GetDrawScale3D", &ActorWrapper::GetDrawScale3D).
		def("SetDrawScale3D", &ActorWrapper::SetDrawScale3D).
		def("GetPrePivot", &ActorWrapper::GetPrePivot).
		def("SetPrePivot", &ActorWrapper::SetPrePivot).
		def("GetCustomTimeDilation", &ActorWrapper::GetCustomTimeDilation).
		def("SetCustomTimeDilation", &ActorWrapper::SetCustomTimeDilation).
		def("GetPhysics", &ActorWrapper::GetPhysics).
		def("SetPhysics", &ActorWrapper::SetPhysics).
		def("GetRemoteRole", &ActorWrapper::GetRemoteRole).
		def("SetRemoteRole", &ActorWrapper::SetRemoteRole).
		def("GetRole", &ActorWrapper::GetRole).
		def("SetRole", &ActorWrapper::SetRole).
		def("GetCollisionType", &ActorWrapper::GetCollisionType).
		def("SetCollisionType", &ActorWrapper::SetCollisionType).
		def("GetReplicatedCollisionType", &ActorWrapper::GetReplicatedCollisionType).
		def("SetReplicatedCollisionType", &ActorWrapper::SetReplicatedCollisionType).
		def("GetOwner", &ActorWrapper::GetOwner).
		def("GetBase", &ActorWrapper::GetBase).
		def("GetbStatic", &ActorWrapper::GetbStatic).
		def("GetbHidden", &ActorWrapper::GetbHidden).
		def("GetbNoDelete", &ActorWrapper::GetbNoDelete).
		def("SetbNoDelete", &ActorWrapper::SetbNoDelete).
		def("GetbDeleteMe", &ActorWrapper::GetbDeleteMe).
		def("SetbDeleteMe", &ActorWrapper::SetbDeleteMe).
		def("GetbTicked", &ActorWrapper::GetbTicked).
		def("SetbTicked", &ActorWrapper::SetbTicked).
		def("GetbOnlyOwnerSee", &ActorWrapper::GetbOnlyOwnerSee).
		def("SetbOnlyOwnerSee", &ActorWrapper::SetbOnlyOwnerSee).
		def("GetbTickIsDisabled", &ActorWrapper::GetbTickIsDisabled).
		def("SetbTickIsDisabled", &ActorWrapper::SetbTickIsDisabled).
		def("GetbWorldGeometry", &ActorWrapper::GetbWorldGeometry).
		def("SetbWorldGeometry", &ActorWrapper::SetbWorldGeometry).
		def("GetbIgnoreRigidBodyPawns", &ActorWrapper::GetbIgnoreRigidBodyPawns).
		def("SetbIgnoreRigidBodyPawns", &ActorWrapper::SetbIgnoreRigidBodyPawns).
		def("GetbOrientOnSlope", &ActorWrapper::GetbOrientOnSlope).
		def("SetbOrientOnSlope", &ActorWrapper::SetbOrientOnSlope).
		def("GetbIsMoving", &ActorWrapper::GetbIsMoving).
		def("GetbAlwaysEncroachCheck", &ActorWrapper::GetbAlwaysEncroachCheck).
		def("SetbAlwaysEncroachCheck", &ActorWrapper::SetbAlwaysEncroachCheck).
		def("GetbHasAlternateTargetLocation", &ActorWrapper::GetbHasAlternateTargetLocation).
		def("GetbAlwaysRelevant", &ActorWrapper::GetbAlwaysRelevant).
		def("GetbGameInstanceRelevant", &ActorWrapper::GetbGameInstanceRelevant).
		def("GetbReplicateInstigator", &ActorWrapper::GetbReplicateInstigator).
		def("GetbReplicateMovement", &ActorWrapper::GetbReplicateMovement).
		def("GetbUpdateSimulatedPosition", &ActorWrapper::GetbUpdateSimulatedPosition).
		def("SetbUpdateSimulatedPosition", &ActorWrapper::SetbUpdateSimulatedPosition).
		def("GetbDemoRecording", &ActorWrapper::GetbDemoRecording).
		def("SetbDemoRecording", &ActorWrapper::SetbDemoRecording).
		def("GetbDemoOwner", &ActorWrapper::GetbDemoOwner).
		def("SetbDemoOwner", &ActorWrapper::SetbDemoOwner).
		def("GetbForceDemoRelevant", &ActorWrapper::GetbForceDemoRelevant).
		def("SetbForceDemoRelevant", &ActorWrapper::SetbForceDemoRelevant).
		def("GetbNetInitialRotation", &ActorWrapper::GetbNetInitialRotation).
		def("SetbNetInitialRotation", &ActorWrapper::SetbNetInitialRotation).
		def("GetbReplicateRigidBodyLocation", &ActorWrapper::GetbReplicateRigidBodyLocation).
		def("SetbReplicateRigidBodyLocation", &ActorWrapper::SetbReplicateRigidBodyLocation).
		def("GetbKillDuringLevelTransition", &ActorWrapper::GetbKillDuringLevelTransition).
		def("SetbKillDuringLevelTransition", &ActorWrapper::SetbKillDuringLevelTransition).
		def("GetbPostRenderIfNotVisible", &ActorWrapper::GetbPostRenderIfNotVisible).
		def("SetbPostRenderIfNotVisible", &ActorWrapper::SetbPostRenderIfNotVisible).
		def("GetbForceNetUpdate", &ActorWrapper::GetbForceNetUpdate).
		def("SetbForceNetUpdate", &ActorWrapper::SetbForceNetUpdate).
		def("GetbForcePacketUpdate", &ActorWrapper::GetbForcePacketUpdate).
		def("SetbForcePacketUpdate", &ActorWrapper::SetbForcePacketUpdate).
		def("GetbPendingNetUpdate", &ActorWrapper::GetbPendingNetUpdate).
		def("SetbPendingNetUpdate", &ActorWrapper::SetbPendingNetUpdate).
		def("GetbGameRelevant", &ActorWrapper::GetbGameRelevant).
		def("SetbGameRelevant", &ActorWrapper::SetbGameRelevant).
		def("GetbMovable", &ActorWrapper::GetbMovable).
		def("SetbMovable", &ActorWrapper::SetbMovable).
		def("GetbCanTeleport", &ActorWrapper::GetbCanTeleport).
		def("SetbCanTeleport", &ActorWrapper::SetbCanTeleport).
		def("GetbAlwaysTick", &ActorWrapper::GetbAlwaysTick).
		def("SetbAlwaysTick", &ActorWrapper::SetbAlwaysTick).
		def("GetbBlocksNavigation", &ActorWrapper::GetbBlocksNavigation).
		def("SetbBlocksNavigation", &ActorWrapper::SetbBlocksNavigation).
		def("GetBlockRigidBody", &ActorWrapper::GetBlockRigidBody).
		def("SetBlockRigidBody", &ActorWrapper::SetBlockRigidBody).
		def("GetbCollideWhenPlacing", &ActorWrapper::GetbCollideWhenPlacing).
		def("SetbCollideWhenPlacing", &ActorWrapper::SetbCollideWhenPlacing).
		def("GetbCollideActors", &ActorWrapper::GetbCollideActors).
		def("SetbCollideActors", &ActorWrapper::SetbCollideActors).
		def("GetbCollideWorld", &ActorWrapper::GetbCollideWorld).
		def("SetbCollideWorld", &ActorWrapper::SetbCollideWorld).
		def("GetbCollideComplex", &ActorWrapper::GetbCollideComplex).
		def("SetbCollideComplex", &ActorWrapper::SetbCollideComplex).
		def("GetbBlockActors", &ActorWrapper::GetbBlockActors).
		def("SetbBlockActors", &ActorWrapper::SetbBlockActors).
		def("GetbBlocksTeleport", &ActorWrapper::GetbBlocksTeleport).
		def("SetbBlocksTeleport", &ActorWrapper::SetbBlocksTeleport).
		def("GetbPhysRigidBodyOutOfWorldCheck", &ActorWrapper::GetbPhysRigidBodyOutOfWorldCheck).
		def("SetbPhysRigidBodyOutOfWorldCheck", &ActorWrapper::SetbPhysRigidBodyOutOfWorldCheck).
		def("GetbComponentOutsideWorld", &ActorWrapper::GetbComponentOutsideWorld).
		def("GetbRigidBodyWasAwake", &ActorWrapper::GetbRigidBodyWasAwake).
		def("SetbRigidBodyWasAwake", &ActorWrapper::SetbRigidBodyWasAwake).
		def("GetbCallRigidBodyWakeEvents", &ActorWrapper::GetbCallRigidBodyWakeEvents).
		def("SetbCallRigidBodyWakeEvents", &ActorWrapper::SetbCallRigidBodyWakeEvents).
		def("GetbBounce", &ActorWrapper::GetbBounce).
		def("SetbBounce", &ActorWrapper::SetbBounce).
		def("GetbEditable", &ActorWrapper::GetbEditable).
		def("SetbEditable", &ActorWrapper::SetbEditable).
		def("GetbLockLocation", &ActorWrapper::GetbLockLocation).
		def("SetbLockLocation", &ActorWrapper::SetbLockLocation).
		def("GetNetUpdateTime", &ActorWrapper::GetNetUpdateTime).
		def("SetNetUpdateTime", &ActorWrapper::SetNetUpdateTime).
		def("GetNetUpdateFrequency", &ActorWrapper::GetNetUpdateFrequency).
		def("SetNetUpdateFrequency", &ActorWrapper::SetNetUpdateFrequency).
		def("GetNetPriority", &ActorWrapper::GetNetPriority).
		def("SetNetPriority", &ActorWrapper::SetNetPriority).
		def("GetLastNetUpdateTime", &ActorWrapper::GetLastNetUpdateTime).
		def("GetLastForcePacketUpdateTime", &ActorWrapper::GetLastForcePacketUpdateTime).
		def("SetLastForcePacketUpdateTime", &ActorWrapper::SetLastForcePacketUpdateTime).
		def("GetTimeSinceLastTick", &ActorWrapper::GetTimeSinceLastTick).
		def("GetLifeSpan", &ActorWrapper::GetLifeSpan).
		def("GetCreationTime", &ActorWrapper::GetCreationTime).
		def("GetLastRenderTime", &ActorWrapper::GetLastRenderTime).
		def("GetCollisionComponent", &ActorWrapper::GetCollisionComponent);


	class_<PrimitiveComponentWrapper, bases<ObjectWrapper>>("PrimitiveComponentWrapper", no_init).
		def("GetRBChannel", &PrimitiveComponentWrapper::GetRBChannel).
		def("SetRBChannel", &PrimitiveComponentWrapper::SetRBChannel).
		def("GetRBDominanceGroup", &PrimitiveComponentWrapper::GetRBDominanceGroup).
		def("SetRBDominanceGroup", &PrimitiveComponentWrapper::SetRBDominanceGroup).
		def("GetbOnlyBlockActorMovement", &PrimitiveComponentWrapper::GetbOnlyBlockActorMovement).
		def("SetbOnlyBlockActorMovement", &PrimitiveComponentWrapper::SetbOnlyBlockActorMovement).
		def("GetHiddenGame", &PrimitiveComponentWrapper::GetHiddenGame).
		def("SetHiddenGame", &PrimitiveComponentWrapper::SetHiddenGame).
		def("GetbOwnerNoSee", &PrimitiveComponentWrapper::GetbOwnerNoSee).
		def("SetbOwnerNoSee", &PrimitiveComponentWrapper::SetbOwnerNoSee).
		def("GetbOnlyOwnerSee", &PrimitiveComponentWrapper::GetbOnlyOwnerSee).
		def("SetbOnlyOwnerSee", &PrimitiveComponentWrapper::SetbOnlyOwnerSee).
		def("GetbIgnoreOwnerHidden", &PrimitiveComponentWrapper::GetbIgnoreOwnerHidden).
		def("SetbIgnoreOwnerHidden", &PrimitiveComponentWrapper::SetbIgnoreOwnerHidden).
		def("GetbUseAsOccluder", &PrimitiveComponentWrapper::GetbUseAsOccluder).
		def("SetbUseAsOccluder", &PrimitiveComponentWrapper::SetbUseAsOccluder).
		def("GetbAllowApproximateOcclusion", &PrimitiveComponentWrapper::GetbAllowApproximateOcclusion).
		def("SetbAllowApproximateOcclusion", &PrimitiveComponentWrapper::SetbAllowApproximateOcclusion).
		def("GetbFirstFrameOcclusion", &PrimitiveComponentWrapper::GetbFirstFrameOcclusion).
		def("SetbFirstFrameOcclusion", &PrimitiveComponentWrapper::SetbFirstFrameOcclusion).
		def("GetbIgnoreNearPlaneIntersection", &PrimitiveComponentWrapper::GetbIgnoreNearPlaneIntersection).
		def("SetbIgnoreNearPlaneIntersection", &PrimitiveComponentWrapper::SetbIgnoreNearPlaneIntersection).
		def("GetbAcceptsStaticDecals", &PrimitiveComponentWrapper::GetbAcceptsStaticDecals).
		def("GetbAcceptsDynamicDecals", &PrimitiveComponentWrapper::GetbAcceptsDynamicDecals).
		def("GetbIsRefreshingDecals", &PrimitiveComponentWrapper::GetbIsRefreshingDecals).
		def("GetCastShadow", &PrimitiveComponentWrapper::GetCastShadow).
		def("SetCastShadow", &PrimitiveComponentWrapper::SetCastShadow).
		def("GetbForceDirectLightMap", &PrimitiveComponentWrapper::GetbForceDirectLightMap).
		def("SetbForceDirectLightMap", &PrimitiveComponentWrapper::SetbForceDirectLightMap).
		def("GetbCastDynamicShadow", &PrimitiveComponentWrapper::GetbCastDynamicShadow).
		def("SetbCastDynamicShadow", &PrimitiveComponentWrapper::SetbCastDynamicShadow).
		def("GetbCastStaticShadow", &PrimitiveComponentWrapper::GetbCastStaticShadow).
		def("SetbCastStaticShadow", &PrimitiveComponentWrapper::SetbCastStaticShadow).
		def("GetbSelfShadowOnly", &PrimitiveComponentWrapper::GetbSelfShadowOnly).
		def("SetbSelfShadowOnly", &PrimitiveComponentWrapper::SetbSelfShadowOnly).
		def("GetbNoModSelfShadow", &PrimitiveComponentWrapper::GetbNoModSelfShadow).
		def("SetbNoModSelfShadow", &PrimitiveComponentWrapper::SetbNoModSelfShadow).
		def("GetbAcceptsDynamicDominantLightShadows", &PrimitiveComponentWrapper::GetbAcceptsDynamicDominantLightShadows).
		def("SetbAcceptsDynamicDominantLightShadows", &PrimitiveComponentWrapper::SetbAcceptsDynamicDominantLightShadows).
		def("GetbCastHiddenShadow", &PrimitiveComponentWrapper::GetbCastHiddenShadow).
		def("SetbCastHiddenShadow", &PrimitiveComponentWrapper::SetbCastHiddenShadow).
		def("GetbCastShadowAsTwoSided", &PrimitiveComponentWrapper::GetbCastShadowAsTwoSided).
		def("SetbCastShadowAsTwoSided", &PrimitiveComponentWrapper::SetbCastShadowAsTwoSided).
		def("GetbAcceptsLights", &PrimitiveComponentWrapper::GetbAcceptsLights).
		def("SetbAcceptsLights", &PrimitiveComponentWrapper::SetbAcceptsLights).
		def("GetbAcceptsDynamicLights", &PrimitiveComponentWrapper::GetbAcceptsDynamicLights).
		def("SetbAcceptsDynamicLights", &PrimitiveComponentWrapper::SetbAcceptsDynamicLights).
		def("GetbUseOnePassLightingOnTranslucency", &PrimitiveComponentWrapper::GetbUseOnePassLightingOnTranslucency).
		def("SetbUseOnePassLightingOnTranslucency", &PrimitiveComponentWrapper::SetbUseOnePassLightingOnTranslucency).
		def("GetbUsePrecomputedShadows", &PrimitiveComponentWrapper::GetbUsePrecomputedShadows).
		def("GetbHasExplicitShadowParent", &PrimitiveComponentWrapper::GetbHasExplicitShadowParent).
		def("GetCollideActors", &PrimitiveComponentWrapper::GetCollideActors).
		def("SetCollideActors", &PrimitiveComponentWrapper::SetCollideActors).
		def("GetAlwaysCheckCollision", &PrimitiveComponentWrapper::GetAlwaysCheckCollision).
		def("SetAlwaysCheckCollision", &PrimitiveComponentWrapper::SetAlwaysCheckCollision).
		def("GetBlockActors", &PrimitiveComponentWrapper::GetBlockActors).
		def("SetBlockActors", &PrimitiveComponentWrapper::SetBlockActors).
		def("GetBlockZeroExtent", &PrimitiveComponentWrapper::GetBlockZeroExtent).
		def("SetBlockZeroExtent", &PrimitiveComponentWrapper::SetBlockZeroExtent).
		def("GetBlockNonZeroExtent", &PrimitiveComponentWrapper::GetBlockNonZeroExtent).
		def("SetBlockNonZeroExtent", &PrimitiveComponentWrapper::SetBlockNonZeroExtent).
		def("GetCanBlockCamera", &PrimitiveComponentWrapper::GetCanBlockCamera).
		def("SetCanBlockCamera", &PrimitiveComponentWrapper::SetCanBlockCamera).
		def("GetBlockRigidBody", &PrimitiveComponentWrapper::GetBlockRigidBody).
		def("SetBlockRigidBody", &PrimitiveComponentWrapper::SetBlockRigidBody).
		def("GetbBlockFootPlacement", &PrimitiveComponentWrapper::GetbBlockFootPlacement).
		def("SetbBlockFootPlacement", &PrimitiveComponentWrapper::SetbBlockFootPlacement).
		def("GetbDisableAllRigidBody", &PrimitiveComponentWrapper::GetbDisableAllRigidBody).
		def("SetbDisableAllRigidBody", &PrimitiveComponentWrapper::SetbDisableAllRigidBody).
		def("GetbSkipRBGeomCreation", &PrimitiveComponentWrapper::GetbSkipRBGeomCreation).
		def("SetbSkipRBGeomCreation", &PrimitiveComponentWrapper::SetbSkipRBGeomCreation).
		def("GetbNotifyRigidBodyCollision", &PrimitiveComponentWrapper::GetbNotifyRigidBodyCollision).
		def("SetbNotifyRigidBodyCollision", &PrimitiveComponentWrapper::SetbNotifyRigidBodyCollision).
		def("GetbFluidDrain", &PrimitiveComponentWrapper::GetbFluidDrain).
		def("SetbFluidDrain", &PrimitiveComponentWrapper::SetbFluidDrain).
		def("GetbFluidTwoWay", &PrimitiveComponentWrapper::GetbFluidTwoWay).
		def("SetbFluidTwoWay", &PrimitiveComponentWrapper::SetbFluidTwoWay).
		def("GetbIgnoreRadialImpulse", &PrimitiveComponentWrapper::GetbIgnoreRadialImpulse).
		def("SetbIgnoreRadialImpulse", &PrimitiveComponentWrapper::SetbIgnoreRadialImpulse).
		def("GetbIgnoreRadialForce", &PrimitiveComponentWrapper::GetbIgnoreRadialForce).
		def("SetbIgnoreRadialForce", &PrimitiveComponentWrapper::SetbIgnoreRadialForce).
		def("GetbIgnoreForceField", &PrimitiveComponentWrapper::GetbIgnoreForceField).
		def("SetbIgnoreForceField", &PrimitiveComponentWrapper::SetbIgnoreForceField).
		def("GetbUseCompartment", &PrimitiveComponentWrapper::GetbUseCompartment).
		def("SetbUseCompartment", &PrimitiveComponentWrapper::SetbUseCompartment).
		def("GetAlwaysLoadOnClient", &PrimitiveComponentWrapper::GetAlwaysLoadOnClient).
		def("SetAlwaysLoadOnClient", &PrimitiveComponentWrapper::SetAlwaysLoadOnClient).
		def("GetAlwaysLoadOnServer", &PrimitiveComponentWrapper::GetAlwaysLoadOnServer).
		def("SetAlwaysLoadOnServer", &PrimitiveComponentWrapper::SetAlwaysLoadOnServer).
		def("GetbIgnoreHiddenActorsMembership", &PrimitiveComponentWrapper::GetbIgnoreHiddenActorsMembership).
		def("SetbIgnoreHiddenActorsMembership", &PrimitiveComponentWrapper::SetbIgnoreHiddenActorsMembership).
		def("GetAbsoluteTranslation", &PrimitiveComponentWrapper::GetAbsoluteTranslation).
		def("SetAbsoluteTranslation", &PrimitiveComponentWrapper::SetAbsoluteTranslation).
		def("GetAbsoluteRotation", &PrimitiveComponentWrapper::GetAbsoluteRotation).
		def("SetAbsoluteRotation", &PrimitiveComponentWrapper::SetAbsoluteRotation).
		def("GetAbsoluteScale", &PrimitiveComponentWrapper::GetAbsoluteScale).
		def("SetAbsoluteScale", &PrimitiveComponentWrapper::SetAbsoluteScale).
		def("GetVisibilityId", &PrimitiveComponentWrapper::GetVisibilityId).
		def("SetVisibilityId", &PrimitiveComponentWrapper::SetVisibilityId).
		def("GetTranslation", &PrimitiveComponentWrapper::GetTranslation).
		def("SetTranslation", &PrimitiveComponentWrapper::SetTranslation).
		def("GetRotation", &PrimitiveComponentWrapper::GetRotation).
		def("SetRotation", &PrimitiveComponentWrapper::SetRotation).
		def("GetScale", &PrimitiveComponentWrapper::GetScale).
		def("SetScale", &PrimitiveComponentWrapper::SetScale).
		def("GetScale3D", &PrimitiveComponentWrapper::GetScale3D).
		def("SetScale3D", &PrimitiveComponentWrapper::SetScale3D).
		def("GetBoundsScale", &PrimitiveComponentWrapper::GetBoundsScale).
		def("SetBoundsScale", &PrimitiveComponentWrapper::SetBoundsScale).
		def("SetLastSubmitTime", &PrimitiveComponentWrapper::SetLastSubmitTime).
		def("GetLastRenderTime", &PrimitiveComponentWrapper::GetLastRenderTime).
		def("GetScriptRigidBodyCollisionThreshold", &PrimitiveComponentWrapper::GetScriptRigidBodyCollisionThreshold).
		def("SetScriptRigidBodyCollisionThreshold", &PrimitiveComponentWrapper::SetScriptRigidBodyCollisionThreshold);


	class_<PlayerReplicationInfoWrapper, bases<ActorWrapper>>("PlayerReplicationInfoWrapper", no_init).
		def("GetScore", &PlayerReplicationInfoWrapper::GetScore).
		def("SetScore", &PlayerReplicationInfoWrapper::SetScore).
		def("GetDeaths", &PlayerReplicationInfoWrapper::GetDeaths).
		def("SetDeaths", &PlayerReplicationInfoWrapper::SetDeaths).
		def("GetPing", &PlayerReplicationInfoWrapper::GetPing).
		def("SetPing", &PlayerReplicationInfoWrapper::SetPing).
		def("GetTTSSpeaker", &PlayerReplicationInfoWrapper::GetTTSSpeaker).
		def("SetTTSSpeaker", &PlayerReplicationInfoWrapper::SetTTSSpeaker).
		def("GetNumLives", &PlayerReplicationInfoWrapper::GetNumLives).
		def("SetNumLives", &PlayerReplicationInfoWrapper::SetNumLives).
		def("GetPlayerID", &PlayerReplicationInfoWrapper::GetPlayerID).
		def("SetPlayerID", &PlayerReplicationInfoWrapper::SetPlayerID).
		def("GetbAdmin", &PlayerReplicationInfoWrapper::GetbAdmin).
		def("SetbAdmin", &PlayerReplicationInfoWrapper::SetbAdmin).
		def("GetbIsSpectator", &PlayerReplicationInfoWrapper::GetbIsSpectator).
		def("SetbIsSpectator", &PlayerReplicationInfoWrapper::SetbIsSpectator).
		def("GetbOnlySpectator", &PlayerReplicationInfoWrapper::GetbOnlySpectator).
		def("SetbOnlySpectator", &PlayerReplicationInfoWrapper::SetbOnlySpectator).
		def("GetbWaitingPlayer", &PlayerReplicationInfoWrapper::GetbWaitingPlayer).
		def("SetbWaitingPlayer", &PlayerReplicationInfoWrapper::SetbWaitingPlayer).
		def("GetbReadyToPlay", &PlayerReplicationInfoWrapper::GetbReadyToPlay).
		def("SetbReadyToPlay", &PlayerReplicationInfoWrapper::SetbReadyToPlay).
		def("GetbOutOfLives", &PlayerReplicationInfoWrapper::GetbOutOfLives).
		def("SetbOutOfLives", &PlayerReplicationInfoWrapper::SetbOutOfLives).
		def("GetbBot", &PlayerReplicationInfoWrapper::GetbBot).
		def("SetbBot", &PlayerReplicationInfoWrapper::SetbBot).
		def("GetbHasBeenWelcomed", &PlayerReplicationInfoWrapper::GetbHasBeenWelcomed).
		def("SetbHasBeenWelcomed", &PlayerReplicationInfoWrapper::SetbHasBeenWelcomed).
		def("GetbIsInactive", &PlayerReplicationInfoWrapper::GetbIsInactive).
		def("SetbIsInactive", &PlayerReplicationInfoWrapper::SetbIsInactive).
		def("GetbFromPreviousLevel", &PlayerReplicationInfoWrapper::GetbFromPreviousLevel).
		def("SetbFromPreviousLevel", &PlayerReplicationInfoWrapper::SetbFromPreviousLevel).
		def("GetbTimedOut", &PlayerReplicationInfoWrapper::GetbTimedOut).
		def("SetbTimedOut", &PlayerReplicationInfoWrapper::SetbTimedOut).
		def("GetbUnregistered", &PlayerReplicationInfoWrapper::GetbUnregistered).
		def("SetbUnregistered", &PlayerReplicationInfoWrapper::SetbUnregistered).
		def("GetStartTime", &PlayerReplicationInfoWrapper::GetStartTime).
		def("SetStartTime", &PlayerReplicationInfoWrapper::SetStartTime).
		def("GetKills", &PlayerReplicationInfoWrapper::GetKills).
		def("SetKills", &PlayerReplicationInfoWrapper::SetKills).
		def("GetExactPing", &PlayerReplicationInfoWrapper::GetExactPing).
		def("SetExactPing", &PlayerReplicationInfoWrapper::SetExactPing);


	class_<RBActorWrapper, bases<ActorWrapper>>("RBActorWrapper", no_init).
		def("GetMaxLinearSpeed", &RBActorWrapper::GetMaxLinearSpeed).
		def("SetMaxLinearSpeed", &RBActorWrapper::SetMaxLinearSpeed).
		def("GetMaxAngularSpeed", &RBActorWrapper::GetMaxAngularSpeed).
		def("SetMaxAngularSpeed", &RBActorWrapper::SetMaxAngularSpeed).
		def("GetbDisableSleeping", &RBActorWrapper::GetbDisableSleeping).
		def("SetbDisableSleeping", &RBActorWrapper::SetbDisableSleeping).
		def("GetbReplayActor", &RBActorWrapper::GetbReplayActor).
		def("SetbReplayActor", &RBActorWrapper::SetbReplayActor).
		def("GetbFrozen", &RBActorWrapper::GetbFrozen).
		def("SetbFrozen", &RBActorWrapper::SetbFrozen).
		def("GetbIgnoreSyncing", &RBActorWrapper::GetbIgnoreSyncing).
		def("SetbIgnoreSyncing", &RBActorWrapper::SetbIgnoreSyncing).
		def("GetbPhysInitialized", &RBActorWrapper::GetbPhysInitialized).
		def("GetOldRBState", &RBActorWrapper::GetOldRBState).
		def("SetOldRBState", &RBActorWrapper::SetOldRBState).
		def("GetRBState", &RBActorWrapper::GetRBState).
		def("SetRBState", &RBActorWrapper::SetRBState).
		def("GetReplicatedRBState", &RBActorWrapper::GetReplicatedRBState).
		def("SetReplicatedRBState", &RBActorWrapper::SetReplicatedRBState).
		def("GetClientCorrectionRBState", &RBActorWrapper::GetClientCorrectionRBState).
		def("SetClientCorrectionRBState", &RBActorWrapper::SetClientCorrectionRBState).
		def("GetWorldContact", &RBActorWrapper::GetWorldContact).
		def("SetWorldContact", &RBActorWrapper::SetWorldContact).
		def("GetSyncErrorLocation", &RBActorWrapper::GetSyncErrorLocation).
		def("GetSyncErrorAngle", &RBActorWrapper::GetSyncErrorAngle).
		def("GetSyncErrorAxis", &RBActorWrapper::GetSyncErrorAxis).
		def("GetLastRBCollisionsFrame", &RBActorWrapper::GetLastRBCollisionsFrame).
		def("GetWeldedActor", &RBActorWrapper::GetWeldedActor).
		def("GetWeldedTo", &RBActorWrapper::GetWeldedTo).
		def("GetPreWeldMass", &RBActorWrapper::GetPreWeldMass);


	class_<BallWrapper, bases<RBActorWrapper>>("BallWrapper", no_init).
		def("GetbAllowPlayerExplosionOverride", &BallWrapper::GetbAllowPlayerExplosionOverride).
		def("SetbAllowPlayerExplosionOverride", &BallWrapper::SetbAllowPlayerExplosionOverride).
		def("GetbNotifyGroundHit", &BallWrapper::GetbNotifyGroundHit).
		def("SetbNotifyGroundHit", &BallWrapper::SetbNotifyGroundHit).
		def("GetbEndOfGameHidden", &BallWrapper::GetbEndOfGameHidden).
		def("SetbEndOfGameHidden", &BallWrapper::SetbEndOfGameHidden).
		def("GetbFadeIn", &BallWrapper::GetbFadeIn).
		def("SetbFadeIn", &BallWrapper::SetbFadeIn).
		def("GetbFadeOut", &BallWrapper::GetbFadeOut).
		def("SetbFadeOut", &BallWrapper::SetbFadeOut).
		def("GetbPredictionOnGround", &BallWrapper::GetbPredictionOnGround).
		def("SetbPredictionOnGround", &BallWrapper::SetbPredictionOnGround).
		def("GetbCanBeAttached", &BallWrapper::GetbCanBeAttached).
		def("SetbCanBeAttached", &BallWrapper::SetbCanBeAttached).
		def("GetbBallDemolish", &BallWrapper::GetbBallDemolish).
		def("SetbBallDemolish", &BallWrapper::SetbBallDemolish).
		def("GetbNewFalling", &BallWrapper::GetbNewFalling).
		def("SetbNewFalling", &BallWrapper::SetbNewFalling).
		def("GetbItemFreeze", &BallWrapper::GetbItemFreeze).
		def("SetbItemFreeze", &BallWrapper::SetbItemFreeze).
		def("GetRadius", &BallWrapper::GetRadius).
		def("SetRadius", &BallWrapper::SetRadius).
		def("GetVisualRadius", &BallWrapper::GetVisualRadius).
		def("SetVisualRadius", &BallWrapper::SetVisualRadius).
		def("GetLastCalculateCarHit", &BallWrapper::GetLastCalculateCarHit).
		def("GetInitialLocation", &BallWrapper::GetInitialLocation).
		def("SetInitialLocation", &BallWrapper::SetInitialLocation).
		def("SetInitialRotation", &BallWrapper::SetInitialRotation).
		def("GetLastHitWorldTime", &BallWrapper::GetLastHitWorldTime).
		def("GetReplicatedBallScale", &BallWrapper::GetReplicatedBallScale).
		def("SetReplicatedBallScale", &BallWrapper::SetReplicatedBallScale).
		def("GetReplicatedWorldBounceScale", &BallWrapper::GetReplicatedWorldBounceScale).
		def("SetReplicatedWorldBounceScale", &BallWrapper::SetReplicatedWorldBounceScale).
		def("GetReplicatedBallGravityScale", &BallWrapper::GetReplicatedBallGravityScale).
		def("SetReplicatedBallGravityScale", &BallWrapper::SetReplicatedBallGravityScale).
		def("GetReplicatedBallMaxLinearSpeedScale", &BallWrapper::GetReplicatedBallMaxLinearSpeedScale).
		def("SetReplicatedBallMaxLinearSpeedScale", &BallWrapper::SetReplicatedBallMaxLinearSpeedScale).
		def("GetReplicatedAddedCarBounceScale", &BallWrapper::GetReplicatedAddedCarBounceScale).
		def("SetReplicatedAddedCarBounceScale", &BallWrapper::SetReplicatedAddedCarBounceScale).
		def("GetAdditionalCarGroundBounceScaleZ", &BallWrapper::GetAdditionalCarGroundBounceScaleZ).
		def("SetAdditionalCarGroundBounceScaleZ", &BallWrapper::SetAdditionalCarGroundBounceScaleZ).
		def("GetAdditionalCarGroundBounceScaleXY", &BallWrapper::GetAdditionalCarGroundBounceScaleXY).
		def("SetAdditionalCarGroundBounceScaleXY", &BallWrapper::SetAdditionalCarGroundBounceScaleXY).
		def("GetHitTeamNum", &BallWrapper::GetHitTeamNum).
		def("SetHitTeamNum", &BallWrapper::SetHitTeamNum).
		def("GetGameEvent", &BallWrapper::GetGameEvent).
		def("GetExplosionTime", &BallWrapper::GetExplosionTime).
		def("SetExplosionTime", &BallWrapper::SetExplosionTime).
		def("GetOldLocation", &BallWrapper::GetOldLocation).
		def("SetOldLocation", &BallWrapper::SetOldLocation).
		def("GetPredictionTimestep", &BallWrapper::GetPredictionTimestep).
		def("SetPredictionTimestep", &BallWrapper::SetPredictionTimestep).
		def("GetLastPredictionTime", &BallWrapper::GetLastPredictionTime).
		def("SetLastPredictionTime", &BallWrapper::SetLastPredictionTime).
		def("GetBallSloMoRadius", &BallWrapper::GetBallSloMoRadius).
		def("SetBallSloMoRadius", &BallWrapper::SetBallSloMoRadius).
		def("GetBallSloMoDuration", &BallWrapper::GetBallSloMoDuration).
		def("SetBallSloMoDuration", &BallWrapper::SetBallSloMoDuration).
		def("GetBallSloMoDilation", &BallWrapper::GetBallSloMoDilation).
		def("SetBallSloMoDilation", &BallWrapper::SetBallSloMoDilation).
		def("GetBallSloMoCooldown", &BallWrapper::GetBallSloMoCooldown).
		def("SetBallSloMoCooldown", &BallWrapper::SetBallSloMoCooldown).
		def("GetBallSloMoNext", &BallWrapper::GetBallSloMoNext).
		def("SetBallSloMoNext", &BallWrapper::SetBallSloMoNext).
		def("GetBallSloMoDiffSpeed", &BallWrapper::GetBallSloMoDiffSpeed).
		def("SetBallSloMoDiffSpeed", &BallWrapper::SetBallSloMoDiffSpeed).
		//def("GetBallBelongsTo", &BallWrapper::GetBallBelongsTo).
		//def("SetBallBelongsTo", &BallWrapper::SetBallBelongsTo).
		def("GetBallTouchScore", &BallWrapper::GetBallTouchScore).
		def("SetBallTouchScore", &BallWrapper::SetBallTouchScore).
		def("GetGroundForce", &BallWrapper::GetGroundForce).
		def("SetGroundForce", &BallWrapper::SetGroundForce).
		def("GetCurrentAffector", &BallWrapper::GetCurrentAffector).
		def("SetCurrentAffector", &BallWrapper::SetCurrentAffector);


	class_<CarComponentWrapper, bases<ActorWrapper>>("CarComponentWrapper", no_init).
		def("GetbDisabled", &CarComponentWrapper::GetbDisabled).
		def("SetbDisabled", &CarComponentWrapper::SetbDisabled).
		def("GetbAutoActivate", &CarComponentWrapper::GetbAutoActivate).
		def("SetbAutoActivate", &CarComponentWrapper::SetbAutoActivate).
		def("GetbSimulateComponent", &CarComponentWrapper::GetbSimulateComponent).
		def("SetbSimulateComponent", &CarComponentWrapper::SetbSimulateComponent).
		def("GetbCreated", &CarComponentWrapper::GetbCreated).
		def("SetbCreated", &CarComponentWrapper::SetbCreated).
		def("GetbActive", &CarComponentWrapper::GetbActive).
		def("SetbActive", &CarComponentWrapper::SetbActive).
		def("GetbRemovedFromCar", &CarComponentWrapper::GetbRemovedFromCar).
		def("SetbRemovedFromCar", &CarComponentWrapper::SetbRemovedFromCar).
		def("GetComponentData", &CarComponentWrapper::GetComponentData).
		def("SetComponentData", &CarComponentWrapper::SetComponentData).
		def("GetReplicatedActive", &CarComponentWrapper::GetReplicatedActive).
		def("SetReplicatedActive", &CarComponentWrapper::SetReplicatedActive).
		def("GetActivator", &CarComponentWrapper::GetActivator).
		def("SetActivator", &CarComponentWrapper::SetActivator).
		def("GetVehicle", &CarComponentWrapper::GetVehicle).
		def("SetVehicle", &CarComponentWrapper::SetVehicle).
		def("GetCar", &CarComponentWrapper::GetCar).
		def("SetCar", &CarComponentWrapper::SetCar).
		def("GetActivityTime", &CarComponentWrapper::GetActivityTime).
		def("SetActivityTime", &CarComponentWrapper::SetActivityTime).
		def("GetReplicatedActivityTime", &CarComponentWrapper::GetReplicatedActivityTime).
		def("SetReplicatedActivityTime", &CarComponentWrapper::SetReplicatedActivityTime);


	class_<AirControlComponentWrapper, bases<CarComponentWrapper>>("AirControlComponentWrapper", no_init).
		def("GetAirTorque", &AirControlComponentWrapper::GetAirTorque).
		def("SetAirTorque", &AirControlComponentWrapper::SetAirTorque).
		def("GetAirDamping", &AirControlComponentWrapper::GetAirDamping).
		def("SetAirDamping", &AirControlComponentWrapper::SetAirDamping).
		def("GetThrottleForce", &AirControlComponentWrapper::GetThrottleForce).
		def("SetThrottleForce", &AirControlComponentWrapper::SetThrottleForce).
		def("GetDodgeDisableTimeRemaining", &AirControlComponentWrapper::GetDodgeDisableTimeRemaining).
		def("SetDodgeDisableTimeRemaining", &AirControlComponentWrapper::SetDodgeDisableTimeRemaining).
		def("GetControlScale", &AirControlComponentWrapper::GetControlScale).
		def("SetControlScale", &AirControlComponentWrapper::SetControlScale).
		def("GetAirControlSensitivity", &AirControlComponentWrapper::GetAirControlSensitivity).
		def("SetAirControlSensitivity", &AirControlComponentWrapper::SetAirControlSensitivity);


	class_<BoostWrapper, bases<CarComponentWrapper>>("BoostWrapper", no_init).
		def("GetBoostConsumptionRate", &BoostWrapper::GetBoostConsumptionRate).
		def("SetBoostConsumptionRate", &BoostWrapper::SetBoostConsumptionRate).
		def("GetMaxBoostAmount", &BoostWrapper::GetMaxBoostAmount).
		def("SetMaxBoostAmount", &BoostWrapper::SetMaxBoostAmount).
		def("GetStartBoostAmount", &BoostWrapper::GetStartBoostAmount).
		def("SetStartBoostAmount", &BoostWrapper::SetStartBoostAmount).
		def("GetCurrentBoostAmount", &BoostWrapper::GetCurrentBoostAmount).
		def("SetCurrentBoostAmount", &BoostWrapper::SetCurrentBoostAmount).
		def("GetBoostModifier", &BoostWrapper::GetBoostModifier).
		def("SetBoostModifier", &BoostWrapper::SetBoostModifier).
		def("GetLastBoostAmountRequestTime", &BoostWrapper::GetLastBoostAmountRequestTime).
		def("SetLastBoostAmountRequestTime", &BoostWrapper::SetLastBoostAmountRequestTime).
		def("GetLastBoostAmount", &BoostWrapper::GetLastBoostAmount).
		def("SetLastBoostAmount", &BoostWrapper::SetLastBoostAmount).
		def("GetBoostForce", &BoostWrapper::GetBoostForce).
		def("SetBoostForce", &BoostWrapper::SetBoostForce).
		def("GetMinBoostTime", &BoostWrapper::GetMinBoostTime).
		def("SetMinBoostTime", &BoostWrapper::SetMinBoostTime).
		def("GetRechargeRate", &BoostWrapper::GetRechargeRate).
		def("SetRechargeRate", &BoostWrapper::SetRechargeRate).
		def("GetRechargeDelay", &BoostWrapper::GetRechargeDelay).
		def("SetRechargeDelay", &BoostWrapper::SetRechargeDelay).
		def("GetUnlimitedBoostRefCount", &BoostWrapper::GetUnlimitedBoostRefCount).
		def("SetUnlimitedBoostRefCount", &BoostWrapper::SetUnlimitedBoostRefCount).
		def("GetbUnlimitedBoost", &BoostWrapper::GetbUnlimitedBoost).
		def("SetbUnlimitedBoost", &BoostWrapper::SetbUnlimitedBoost).
		def("GetbNoBoost", &BoostWrapper::GetbNoBoost).
		def("SetbNoBoost", &BoostWrapper::SetbNoBoost).
		def("GetReplicatedBoostAmount", &BoostWrapper::GetReplicatedBoostAmount).
		def("SetReplicatedBoostAmount", &BoostWrapper::SetReplicatedBoostAmount);


	class_<DodgeComponentWrapper, bases<CarComponentWrapper>>("DodgeComponentWrapper", no_init).
		def("GetDodgeInputThreshold", &DodgeComponentWrapper::GetDodgeInputThreshold).
		def("SetDodgeInputThreshold", &DodgeComponentWrapper::SetDodgeInputThreshold).
		def("GetSideDodgeImpulse", &DodgeComponentWrapper::GetSideDodgeImpulse).
		def("SetSideDodgeImpulse", &DodgeComponentWrapper::SetSideDodgeImpulse).
		def("GetSideDodgeImpulseMaxSpeedScale", &DodgeComponentWrapper::GetSideDodgeImpulseMaxSpeedScale).
		def("SetSideDodgeImpulseMaxSpeedScale", &DodgeComponentWrapper::SetSideDodgeImpulseMaxSpeedScale).
		def("GetForwardDodgeImpulse", &DodgeComponentWrapper::GetForwardDodgeImpulse).
		def("SetForwardDodgeImpulse", &DodgeComponentWrapper::SetForwardDodgeImpulse).
		def("GetForwardDodgeImpulseMaxSpeedScale", &DodgeComponentWrapper::GetForwardDodgeImpulseMaxSpeedScale).
		def("SetForwardDodgeImpulseMaxSpeedScale", &DodgeComponentWrapper::SetForwardDodgeImpulseMaxSpeedScale).
		def("GetBackwardDodgeImpulse", &DodgeComponentWrapper::GetBackwardDodgeImpulse).
		def("SetBackwardDodgeImpulse", &DodgeComponentWrapper::SetBackwardDodgeImpulse).
		def("GetBackwardDodgeImpulseMaxSpeedScale", &DodgeComponentWrapper::GetBackwardDodgeImpulseMaxSpeedScale).
		def("SetBackwardDodgeImpulseMaxSpeedScale", &DodgeComponentWrapper::SetBackwardDodgeImpulseMaxSpeedScale).
		def("GetSideDodgeTorque", &DodgeComponentWrapper::GetSideDodgeTorque).
		def("SetSideDodgeTorque", &DodgeComponentWrapper::SetSideDodgeTorque).
		def("GetForwardDodgeTorque", &DodgeComponentWrapper::GetForwardDodgeTorque).
		def("SetForwardDodgeTorque", &DodgeComponentWrapper::SetForwardDodgeTorque).
		def("GetDodgeTorqueTime", &DodgeComponentWrapper::GetDodgeTorqueTime).
		def("SetDodgeTorqueTime", &DodgeComponentWrapper::SetDodgeTorqueTime).
		def("GetMinDodgeTorqueTime", &DodgeComponentWrapper::GetMinDodgeTorqueTime).
		def("SetMinDodgeTorqueTime", &DodgeComponentWrapper::SetMinDodgeTorqueTime).
		def("GetDodgeZDamping", &DodgeComponentWrapper::GetDodgeZDamping).
		def("SetDodgeZDamping", &DodgeComponentWrapper::SetDodgeZDamping).
		def("GetDodgeZDampingDelay", &DodgeComponentWrapper::GetDodgeZDampingDelay).
		def("SetDodgeZDampingDelay", &DodgeComponentWrapper::SetDodgeZDampingDelay).
		def("GetDodgeZDampingUpTime", &DodgeComponentWrapper::GetDodgeZDampingUpTime).
		def("SetDodgeZDampingUpTime", &DodgeComponentWrapper::SetDodgeZDampingUpTime).
		def("GetDodgeImpulseScale", &DodgeComponentWrapper::GetDodgeImpulseScale).
		def("SetDodgeImpulseScale", &DodgeComponentWrapper::SetDodgeImpulseScale).
		def("GetDodgeTorqueScale", &DodgeComponentWrapper::GetDodgeTorqueScale).
		def("SetDodgeTorqueScale", &DodgeComponentWrapper::SetDodgeTorqueScale).
		def("GetDodgeTorque", &DodgeComponentWrapper::GetDodgeTorque).
		def("SetDodgeTorque", &DodgeComponentWrapper::SetDodgeTorque).
		def("GetDodgeDirection", &DodgeComponentWrapper::GetDodgeDirection).
		def("SetDodgeDirection", &DodgeComponentWrapper::SetDodgeDirection);


	class_<DoubleJumpComponentWrapper, bases<CarComponentWrapper>>("DoubleJumpComponentWrapper", no_init).
		def("SetJumpImpulse", &DoubleJumpComponentWrapper::SetJumpImpulse).
		def("GetImpulseScale", &DoubleJumpComponentWrapper::GetImpulseScale).
		def("SetImpulseScale", &DoubleJumpComponentWrapper::SetImpulseScale);


	class_<FlipCarComponentWrapper, bases<CarComponentWrapper>>("FlipCarComponentWrapper", no_init).
		def("GetFlipCarImpulse", &FlipCarComponentWrapper::GetFlipCarImpulse).
		def("SetFlipCarImpulse", &FlipCarComponentWrapper::SetFlipCarImpulse).
		def("GetFlipCarTorque", &FlipCarComponentWrapper::GetFlipCarTorque).
		def("SetFlipCarTorque", &FlipCarComponentWrapper::SetFlipCarTorque).
		def("GetFlipCarTime", &FlipCarComponentWrapper::GetFlipCarTime).
		def("SetFlipCarTime", &FlipCarComponentWrapper::SetFlipCarTime).
		def("GetbFlipRight", &FlipCarComponentWrapper::GetbFlipRight).
		def("SetbFlipRight", &FlipCarComponentWrapper::SetbFlipRight);


	class_<JumpComponentWrapper, bases<CarComponentWrapper>>("JumpComponentWrapper", no_init).
		def("GetMinJumpTime", &JumpComponentWrapper::GetMinJumpTime).
		def("SetMinJumpTime", &JumpComponentWrapper::SetMinJumpTime).
		def("GetJumpImpulse", &JumpComponentWrapper::GetJumpImpulse).
		def("SetJumpImpulse", &JumpComponentWrapper::SetJumpImpulse).
		def("GetJumpForce", &JumpComponentWrapper::GetJumpForce).
		def("SetJumpForce", &JumpComponentWrapper::SetJumpForce).
		def("GetJumpForceTime", &JumpComponentWrapper::GetJumpForceTime).
		def("SetJumpForceTime", &JumpComponentWrapper::SetJumpForceTime).
		def("GetPodiumJumpForceTime", &JumpComponentWrapper::GetPodiumJumpForceTime).
		def("SetPodiumJumpForceTime", &JumpComponentWrapper::SetPodiumJumpForceTime).
		def("GetJumpImpulseSpeed", &JumpComponentWrapper::GetJumpImpulseSpeed).
		def("SetJumpImpulseSpeed", &JumpComponentWrapper::SetJumpImpulseSpeed).
		def("GetJumpAccel", &JumpComponentWrapper::GetJumpAccel).
		def("SetJumpAccel", &JumpComponentWrapper::SetJumpAccel).
		def("GetMaxJumpHeight", &JumpComponentWrapper::GetMaxJumpHeight).
		def("SetMaxJumpHeight", &JumpComponentWrapper::SetMaxJumpHeight).
		def("GetMaxJumpHeightTime", &JumpComponentWrapper::GetMaxJumpHeightTime).
		def("SetMaxJumpHeightTime", &JumpComponentWrapper::SetMaxJumpHeightTime).
		def("GetbDeactivate", &JumpComponentWrapper::GetbDeactivate).
		def("SetbDeactivate", &JumpComponentWrapper::SetbDeactivate);


	class_<RumblePickupComponentWrapper, bases<CarComponentWrapper>>("RumblePickupComponentWrapper", no_init).
		def("GetbHudIgnoreUseTime", &RumblePickupComponentWrapper::GetbHudIgnoreUseTime).
		def("SetbHudIgnoreUseTime", &RumblePickupComponentWrapper::SetbHudIgnoreUseTime).
		def("GetbHasActivated", &RumblePickupComponentWrapper::GetbHasActivated).
		def("SetbHasActivated", &RumblePickupComponentWrapper::SetbHasActivated).
		def("GetbIsActive", &RumblePickupComponentWrapper::GetbIsActive).
		def("SetbIsActive", &RumblePickupComponentWrapper::SetbIsActive).
		def("GetActivationDuration", &RumblePickupComponentWrapper::GetActivationDuration).
		def("SetActivationDuration", &RumblePickupComponentWrapper::SetActivationDuration);


	class_<GravityPickup, bases<RumblePickupComponentWrapper>>("GravityPickup", no_init).
		def("GetBallGravity", &GravityPickup::GetBallGravity).
		def("SetBallGravity", &GravityPickup::SetBallGravity).
		def("GetRange", &GravityPickup::GetRange).
		def("SetRange", &GravityPickup::SetRange).
		def("GetOffset", &GravityPickup::GetOffset).
		def("SetOffset", &GravityPickup::SetOffset).
		def("GetbDeactivateOnTouch", &GravityPickup::GetbDeactivateOnTouch).
		def("SetbDeactivateOnTouch", &GravityPickup::SetbDeactivateOnTouch).
		def("GetRecordBallHitRate", &GravityPickup::GetRecordBallHitRate).
		def("SetRecordBallHitRate", &GravityPickup::SetRecordBallHitRate).
		def("GetLastRecordedBallHitTime", &GravityPickup::GetLastRecordedBallHitTime).
		def("SetLastRecordedBallHitTime", &GravityPickup::SetLastRecordedBallHitTime).
		def("GetPrevBall", &GravityPickup::GetPrevBall).
		def("SetPrevBall", &GravityPickup::SetPrevBall);


	class_<TargetedPickup, bases<RumblePickupComponentWrapper>>("TargetedPickup", no_init).
		def("GetbCanTargetBall", &TargetedPickup::GetbCanTargetBall).
		def("SetbCanTargetBall", &TargetedPickup::SetbCanTargetBall).
		def("GetbCanTargetCars", &TargetedPickup::GetbCanTargetCars).
		def("SetbCanTargetCars", &TargetedPickup::SetbCanTargetCars).
		def("GetbCanTargetEnemyCars", &TargetedPickup::GetbCanTargetEnemyCars).
		def("SetbCanTargetEnemyCars", &TargetedPickup::SetbCanTargetEnemyCars).
		def("GetbCanTargetTeamCars", &TargetedPickup::GetbCanTargetTeamCars).
		def("SetbCanTargetTeamCars", &TargetedPickup::SetbCanTargetTeamCars).
		def("GetbUseDirectionalTargeting", &TargetedPickup::GetbUseDirectionalTargeting).
		def("SetbUseDirectionalTargeting", &TargetedPickup::SetbUseDirectionalTargeting).
		def("GetbRequireTrace", &TargetedPickup::GetbRequireTrace).
		def("SetbRequireTrace", &TargetedPickup::SetbRequireTrace).
		def("GetRange", &TargetedPickup::GetRange).
		def("SetRange", &TargetedPickup::SetRange).
		def("GetDirectionalTargetingAccuracy", &TargetedPickup::GetDirectionalTargetingAccuracy).
		def("SetDirectionalTargetingAccuracy", &TargetedPickup::SetDirectionalTargetingAccuracy).
		def("GetClientTarget", &TargetedPickup::GetClientTarget).
		def("SetClientTarget", &TargetedPickup::SetClientTarget).
		def("GetTargeted", &TargetedPickup::GetTargeted).
		def("SetTargeted", &TargetedPickup::SetTargeted);


	class_<BallFreezePickup, bases<TargetedPickup>>("BallFreezePickup", no_init).
		def("GetbMaintainMomentum", &BallFreezePickup::GetbMaintainMomentum).
		def("SetbMaintainMomentum", &BallFreezePickup::SetbMaintainMomentum).
		def("GetbTouched", &BallFreezePickup::GetbTouched).
		def("SetbTouched", &BallFreezePickup::SetbTouched).
		def("GetTimeToStop", &BallFreezePickup::GetTimeToStop).
		def("SetTimeToStop", &BallFreezePickup::SetTimeToStop).
		def("GetStopMomentumPercentage", &BallFreezePickup::GetStopMomentumPercentage).
		def("SetStopMomentumPercentage", &BallFreezePickup::SetStopMomentumPercentage).
		def("GetBall", &BallFreezePickup::GetBall).
		def("SetBall", &BallFreezePickup::SetBall).
		def("GetOrigLinearVelocity", &BallFreezePickup::GetOrigLinearVelocity).
		def("SetOrigLinearVelocity", &BallFreezePickup::SetOrigLinearVelocity).
		def("GetOrigAngularVelocity", &BallFreezePickup::GetOrigAngularVelocity).
		def("SetOrigAngularVelocity", &BallFreezePickup::SetOrigAngularVelocity).
		def("GetOrigSpeed", &BallFreezePickup::GetOrigSpeed).
		def("SetOrigSpeed", &BallFreezePickup::SetOrigSpeed).
		def("GetRepOrigSpeed", &BallFreezePickup::GetRepOrigSpeed).
		def("SetRepOrigSpeed", &BallFreezePickup::SetRepOrigSpeed);


	class_<GrapplingHookPickup, bases<TargetedPickup>>("GrapplingHookPickup", no_init).
		def("GetImpulse", &GrapplingHookPickup::GetImpulse).
		def("SetImpulse", &GrapplingHookPickup::SetImpulse).
		def("GetForce", &GrapplingHookPickup::GetForce).
		def("SetForce", &GrapplingHookPickup::SetForce).
		def("GetMaxRopeLength", &GrapplingHookPickup::GetMaxRopeLength).
		def("SetMaxRopeLength", &GrapplingHookPickup::SetMaxRopeLength).
		def("GetPredictionSpeed", &GrapplingHookPickup::GetPredictionSpeed).
		def("SetPredictionSpeed", &GrapplingHookPickup::SetPredictionSpeed).
		def("GetbDeactivateOnTouch", &GrapplingHookPickup::GetbDeactivateOnTouch).
		def("SetbDeactivateOnTouch", &GrapplingHookPickup::SetbDeactivateOnTouch).
		def("GetbInstant", &GrapplingHookPickup::GetbInstant).
		def("SetbInstant", &GrapplingHookPickup::SetbInstant).
		def("GetbBlocked", &GrapplingHookPickup::GetbBlocked).
		def("SetbBlocked", &GrapplingHookPickup::SetbBlocked).
		def("GetbAttachedToBall", &GrapplingHookPickup::GetbAttachedToBall).
		def("SetbAttachedToBall", &GrapplingHookPickup::SetbAttachedToBall).
		def("GetRopeMeshScale", &GrapplingHookPickup::GetRopeMeshScale).
		def("SetRopeMeshScale", &GrapplingHookPickup::SetRopeMeshScale).
		def("GetRopeMeshInitialSize", &GrapplingHookPickup::GetRopeMeshInitialSize).
		def("SetRopeMeshInitialSize", &GrapplingHookPickup::SetRopeMeshInitialSize).
		def("GetRopeRotationOffset", &GrapplingHookPickup::GetRopeRotationOffset).
		def("SetRopeRotationOffset", &GrapplingHookPickup::SetRopeRotationOffset).
		def("GetHookMeshScale", &GrapplingHookPickup::GetHookMeshScale).
		def("SetHookMeshScale", &GrapplingHookPickup::SetHookMeshScale).
		def("GetHookMeshOffset", &GrapplingHookPickup::GetHookMeshOffset).
		def("SetHookMeshOffset", &GrapplingHookPickup::SetHookMeshOffset).
		def("GetHookRotationOffset", &GrapplingHookPickup::GetHookRotationOffset).
		def("SetHookRotationOffset", &GrapplingHookPickup::SetHookRotationOffset).
		def("GetHitDistanceOffset", &GrapplingHookPickup::GetHitDistanceOffset).
		def("SetHitDistanceOffset", &GrapplingHookPickup::SetHitDistanceOffset).
		def("GetAfterAttachDuration", &GrapplingHookPickup::GetAfterAttachDuration).
		def("SetAfterAttachDuration", &GrapplingHookPickup::SetAfterAttachDuration).
		def("GetBlockedRequiredMoveDistance", &GrapplingHookPickup::GetBlockedRequiredMoveDistance).
		def("SetBlockedRequiredMoveDistance", &GrapplingHookPickup::SetBlockedRequiredMoveDistance).
		def("GetBlockedRequiredMoveTime", &GrapplingHookPickup::GetBlockedRequiredMoveTime).
		def("SetBlockedRequiredMoveTime", &GrapplingHookPickup::SetBlockedRequiredMoveTime).
		def("GetBlockedStartTime", &GrapplingHookPickup::GetBlockedStartTime).
		def("SetBlockedStartTime", &GrapplingHookPickup::SetBlockedStartTime).
		def("GetBlockedStartPos", &GrapplingHookPickup::GetBlockedStartPos).
		def("SetBlockedStartPos", &GrapplingHookPickup::SetBlockedStartPos).
		def("GetBall", &GrapplingHookPickup::GetBall).
		def("SetBall", &GrapplingHookPickup::SetBall).
		def("GetRopeOrigin", &GrapplingHookPickup::GetRopeOrigin).
		def("SetRopeOrigin", &GrapplingHookPickup::SetRopeOrigin).
		def("GetRopeToTime", &GrapplingHookPickup::GetRopeToTime).
		def("SetRopeToTime", &GrapplingHookPickup::SetRopeToTime).
		def("GetCurrentRopeLength", &GrapplingHookPickup::GetCurrentRopeLength).
		def("SetCurrentRopeLength", &GrapplingHookPickup::SetCurrentRopeLength).
		def("GetAttachTime", &GrapplingHookPickup::GetAttachTime).
		def("SetAttachTime", &GrapplingHookPickup::SetAttachTime);


	class_<SpringPickup, bases<TargetedPickup>>("SpringPickup", no_init).
		def("GetForce", &SpringPickup::GetForce).
		def("SetForce", &SpringPickup::SetForce).
		def("GetVerticalForce", &SpringPickup::GetVerticalForce).
		def("SetVerticalForce", &SpringPickup::SetVerticalForce).
		def("GetTorque", &SpringPickup::GetTorque).
		def("SetTorque", &SpringPickup::SetTorque).
		def("GetbApplyRelativeForce", &SpringPickup::GetbApplyRelativeForce).
		def("SetbApplyRelativeForce", &SpringPickup::SetbApplyRelativeForce).
		def("GetbApplyConstantForce", &SpringPickup::GetbApplyConstantForce).
		def("SetbApplyConstantForce", &SpringPickup::SetbApplyConstantForce).
		def("GetbBreakConstantForceWithHit", &SpringPickup::GetbBreakConstantForceWithHit).
		def("SetbBreakConstantForceWithHit", &SpringPickup::SetbBreakConstantForceWithHit).
		def("GetbApplyRelativeConstantForce", &SpringPickup::GetbApplyRelativeConstantForce).
		def("SetbApplyRelativeConstantForce", &SpringPickup::SetbApplyRelativeConstantForce).
		def("GetbInstant", &SpringPickup::GetbInstant).
		def("SetbInstant", &SpringPickup::SetbInstant).
		def("GetbFollowAfterHit", &SpringPickup::GetbFollowAfterHit).
		def("SetbFollowAfterHit", &SpringPickup::SetbFollowAfterHit).
		def("GetbSpringed", &SpringPickup::GetbSpringed).
		def("SetbSpringed", &SpringPickup::SetbSpringed).
		def("GetRelativeForceNormalDirection", &SpringPickup::GetRelativeForceNormalDirection).
		def("SetRelativeForceNormalDirection", &SpringPickup::SetRelativeForceNormalDirection).
		def("GetMaxSpringLength", &SpringPickup::GetMaxSpringLength).
		def("SetMaxSpringLength", &SpringPickup::SetMaxSpringLength).
		def("GetConstantForce", &SpringPickup::GetConstantForce).
		def("SetConstantForce", &SpringPickup::SetConstantForce).
		def("GetFromOffset", &SpringPickup::GetFromOffset).
		def("SetFromOffset", &SpringPickup::SetFromOffset).
		def("GetSpringMeshScale", &SpringPickup::GetSpringMeshScale).
		def("SetSpringMeshScale", &SpringPickup::SetSpringMeshScale).
		def("GetSpringMeshInitialSize", &SpringPickup::GetSpringMeshInitialSize).
		def("SetSpringMeshInitialSize", &SpringPickup::SetSpringMeshInitialSize).
		def("GetSpringRotationOffset", &SpringPickup::GetSpringRotationOffset).
		def("SetSpringRotationOffset", &SpringPickup::SetSpringRotationOffset).
		def("GetHittingMeshScale", &SpringPickup::GetHittingMeshScale).
		def("SetHittingMeshScale", &SpringPickup::SetHittingMeshScale).
		def("GetHittingMeshOffset", &SpringPickup::GetHittingMeshOffset).
		def("SetHittingMeshOffset", &SpringPickup::SetHittingMeshOffset).
		def("GetHittingRotationOffset", &SpringPickup::GetHittingRotationOffset).
		def("SetHittingRotationOffset", &SpringPickup::SetHittingRotationOffset).
		def("GetHitDistanceOffset", &SpringPickup::GetHitDistanceOffset).
		def("SetHitDistanceOffset", &SpringPickup::SetHitDistanceOffset).
		def("GetAfterSpringDuration", &SpringPickup::GetAfterSpringDuration).
		def("SetAfterSpringDuration", &SpringPickup::SetAfterSpringDuration).
		def("GetBallHitType", &SpringPickup::GetBallHitType).
		def("SetBallHitType", &SpringPickup::SetBallHitType).
		def("GetMinSpringLength", &SpringPickup::GetMinSpringLength).
		def("SetMinSpringLength", &SpringPickup::SetMinSpringLength).
		def("GetWeldedForceScalar", &SpringPickup::GetWeldedForceScalar).
		def("SetWeldedForceScalar", &SpringPickup::SetWeldedForceScalar).
		def("GetWeldedVerticalForce", &SpringPickup::GetWeldedVerticalForce).
		def("SetWeldedVerticalForce", &SpringPickup::SetWeldedVerticalForce).
		def("GetCurrentSpringLength", &SpringPickup::GetCurrentSpringLength).
		def("SetCurrentSpringLength", &SpringPickup::SetCurrentSpringLength).
		def("GetSpringedTime", &SpringPickup::GetSpringedTime).
		def("SetSpringedTime", &SpringPickup::SetSpringedTime).
		def("GetAfterSpringTime", &SpringPickup::GetAfterSpringTime).
		def("SetAfterSpringTime", &SpringPickup::SetAfterSpringTime).
		def("GetSpringToTime", &SpringPickup::GetSpringToTime).
		def("SetSpringToTime", &SpringPickup::SetSpringToTime).
		def("GetSpringOrigin", &SpringPickup::GetSpringOrigin).
		def("SetSpringOrigin", &SpringPickup::SetSpringOrigin).
		def("GetSpringedLocation", &SpringPickup::GetSpringedLocation).
		def("SetSpringedLocation", &SpringPickup::SetSpringedLocation).
		def("GetSpringedNormal", &SpringPickup::GetSpringedNormal).
		def("SetSpringedNormal", &SpringPickup::SetSpringedNormal).
		def("GetSpringedLength", &SpringPickup::GetSpringedLength).
		def("SetSpringedLength", &SpringPickup::SetSpringedLength);


	class_<BallLassoPickup, bases<SpringPickup>>("BallLassoPickup", no_init);


	class_<TornadoPickup, bases<RumblePickupComponentWrapper>>("TornadoPickup", no_init).
		def("GetHeight", &TornadoPickup::GetHeight).
		def("SetHeight", &TornadoPickup::SetHeight).
		def("GetRadius", &TornadoPickup::GetRadius).
		def("SetRadius", &TornadoPickup::SetRadius).
		def("GetOffset", &TornadoPickup::GetOffset).
		def("SetOffset", &TornadoPickup::SetOffset).
		def("GetRotationalForce", &TornadoPickup::GetRotationalForce).
		def("SetRotationalForce", &TornadoPickup::SetRotationalForce).
		def("GetTorque", &TornadoPickup::GetTorque).
		def("SetTorque", &TornadoPickup::SetTorque).
		def("GetFXScale", &TornadoPickup::GetFXScale).
		def("SetFXScale", &TornadoPickup::SetFXScale).
		def("GetFXOffset", &TornadoPickup::GetFXOffset).
		def("SetFXOffset", &TornadoPickup::SetFXOffset).
		def("GetMeshOffset", &TornadoPickup::GetMeshOffset).
		def("SetMeshOffset", &TornadoPickup::SetMeshOffset).
		def("GetMeshScale", &TornadoPickup::GetMeshScale).
		def("SetMeshScale", &TornadoPickup::SetMeshScale).
		def("GetMaxVelocityOffset", &TornadoPickup::GetMaxVelocityOffset).
		def("SetMaxVelocityOffset", &TornadoPickup::SetMaxVelocityOffset).
		def("GetBallMultiplier", &TornadoPickup::GetBallMultiplier).
		def("SetBallMultiplier", &TornadoPickup::SetBallMultiplier).
		def("GetbDebugVis", &TornadoPickup::GetbDebugVis).
		def("SetbDebugVis", &TornadoPickup::SetbDebugVis).
		def("GetVelocityEase", &TornadoPickup::GetVelocityEase).
		def("SetVelocityEase", &TornadoPickup::SetVelocityEase).
		def("GetVel", &TornadoPickup::GetVel).
		def("SetVel", &TornadoPickup::SetVel).
		def("GetAffecting", &TornadoPickup::GetAffecting);


	class_<GameEventWrapper, bases<ActorWrapper>>("GameEventWrapper", no_init).

		def("GetGameMode", &GameEventWrapper::GetGameMode).
		def("SetGameMode", &GameEventWrapper::SetGameMode).
		def("GetReplicatedStateIndex", &GameEventWrapper::GetReplicatedStateIndex).
		def("SetReplicatedStateIndex", &GameEventWrapper::SetReplicatedStateIndex).
		def("GetCarArchetype", &GameEventWrapper::GetCarArchetype).
		def("SetCarArchetype", &GameEventWrapper::SetCarArchetype).
		def("GetCountdownTime", &GameEventWrapper::GetCountdownTime).
		def("SetCountdownTime", &GameEventWrapper::SetCountdownTime).
		def("GetFinishTime", &GameEventWrapper::GetFinishTime).
		def("SetFinishTime", &GameEventWrapper::SetFinishTime).
		def("GetbMultiplayer", &GameEventWrapper::GetbMultiplayer).
		def("SetbMultiplayer", &GameEventWrapper::SetbMultiplayer).
		def("GetbFillWithAI", &GameEventWrapper::GetbFillWithAI).
		def("SetbFillWithAI", &GameEventWrapper::SetbFillWithAI).
		def("GetbAllowReadyUp", &GameEventWrapper::GetbAllowReadyUp).
		def("SetbAllowReadyUp", &GameEventWrapper::SetbAllowReadyUp).
		def("GetbRestartingMatch", &GameEventWrapper::GetbRestartingMatch).
		def("SetbRestartingMatch", &GameEventWrapper::SetbRestartingMatch).
		def("GetbRandomizedBotLoadouts", &GameEventWrapper::GetbRandomizedBotLoadouts).
		def("SetbRandomizedBotLoadouts", &GameEventWrapper::SetbRandomizedBotLoadouts).
		def("GetbHasLeaveMatchPenalty", &GameEventWrapper::GetbHasLeaveMatchPenalty).
		def("SetbHasLeaveMatchPenalty", &GameEventWrapper::SetbHasLeaveMatchPenalty).
		def("GetbCanVoteToForfeit", &GameEventWrapper::GetbCanVoteToForfeit).
		def("SetbCanVoteToForfeit", &GameEventWrapper::SetbCanVoteToForfeit).
		def("GetbDisableAimAssist", &GameEventWrapper::GetbDisableAimAssist).
		def("SetbDisableAimAssist", &GameEventWrapper::SetbDisableAimAssist).
		def("GetMinPlayers", &GameEventWrapper::GetMinPlayers).
		def("SetMinPlayers", &GameEventWrapper::SetMinPlayers).
		def("GetMaxPlayers", &GameEventWrapper::GetMaxPlayers).
		def("SetMaxPlayers", &GameEventWrapper::SetMaxPlayers).
		def("GetSpawnPoints", &GameEventWrapper::GetSpawnPoints).
		def("GetBotSkill", &GameEventWrapper::GetBotSkill).
		def("SetBotSkill", &GameEventWrapper::SetBotSkill).
		def("GetRespawnTime", &GameEventWrapper::GetRespawnTime).
		def("SetRespawnTime", &GameEventWrapper::SetRespawnTime).
		def("GetMatchTimeDilation", &GameEventWrapper::GetMatchTimeDilation).
		def("SetMatchTimeDilation", &GameEventWrapper::SetMatchTimeDilation).
		def("GetActivatorCar", &GameEventWrapper::GetActivatorCar).
		def("SetActivatorCar", &GameEventWrapper::SetActivatorCar).
		def("GetPRIs", &GameEventWrapper::GetPRIs).
		def("GetCars", &GameEventWrapper::GetCars).
		def("GetStartPointIndex", &GameEventWrapper::GetStartPointIndex).
		def("SetStartPointIndex", &GameEventWrapper::SetStartPointIndex).
		def("GetGameStateTimeRemaining", &GameEventWrapper::GetGameStateTimeRemaining).
		def("SetGameStateTimeRemaining", &GameEventWrapper::SetGameStateTimeRemaining).
		def("GetReplicatedGameStateTimeRemaining", &GameEventWrapper::GetReplicatedGameStateTimeRemaining).
		def("SetReplicatedGameStateTimeRemaining", &GameEventWrapper::SetReplicatedGameStateTimeRemaining).
		def("GetIdleKickTime", &GameEventWrapper::GetIdleKickTime).
		def("SetIdleKickTime", &GameEventWrapper::SetIdleKickTime).
		def("GetIdleKickWarningTime", &GameEventWrapper::GetIdleKickWarningTime).
		def("SetIdleKickWarningTime", &GameEventWrapper::SetIdleKickWarningTime).
		def("GetGameOwner", &GameEventWrapper::GetGameOwner).
		def("SetGameOwner", &GameEventWrapper::SetGameOwner).
		def("GetReplicatedRoundCountDownNumber", &GameEventWrapper::GetReplicatedRoundCountDownNumber).
		def("SetReplicatedRoundCountDownNumber", &GameEventWrapper::SetReplicatedRoundCountDownNumber);


	class_<TeamGameEventWrapper, bases<GameEventWrapper>>("TeamGameEventWrapper", no_init).
		def("GetTeamArchetypes", &TeamGameEventWrapper::GetTeamArchetypes).
		def("GetTeams", &TeamGameEventWrapper::GetTeams).
		def("GetMaxTeamSize", &TeamGameEventWrapper::GetMaxTeamSize).
		def("SetMaxTeamSize", &TeamGameEventWrapper::SetMaxTeamSize).
		def("GetNumBots", &TeamGameEventWrapper::GetNumBots).
		def("SetNumBots", &TeamGameEventWrapper::SetNumBots).
		def("GetbMuteOppositeTeams", &TeamGameEventWrapper::GetbMuteOppositeTeams).
		def("SetbMuteOppositeTeams", &TeamGameEventWrapper::SetbMuteOppositeTeams).
		def("GetbDisableMutingOtherTeam", &TeamGameEventWrapper::GetbDisableMutingOtherTeam).
		def("SetbDisableMutingOtherTeam", &TeamGameEventWrapper::SetbDisableMutingOtherTeam).
		def("GetbForfeit", &TeamGameEventWrapper::GetbForfeit).
		def("SetbForfeit", &TeamGameEventWrapper::SetbForfeit).
		def("GetbUnfairTeams", &TeamGameEventWrapper::GetbUnfairTeams).
		def("SetbUnfairTeams", &TeamGameEventWrapper::SetbUnfairTeams).
		def("GetbAlwaysAutoSelectTeam", &TeamGameEventWrapper::GetbAlwaysAutoSelectTeam).
		def("SetbAlwaysAutoSelectTeam", &TeamGameEventWrapper::SetbAlwaysAutoSelectTeam);


	class_<GoalWrapper, bases<ObjectWrapper>>("GoalWrapper", no_init).
		def("GetGoalOrientation", &GoalWrapper::GetGoalOrientation).
		def("SetGoalOrientation", &GoalWrapper::SetGoalOrientation).
		def("GetOverrideGoalIndicatorOrientations", &GoalWrapper::GetOverrideGoalIndicatorOrientations).
		def("GetTeamNum", &GoalWrapper::GetTeamNum).
		def("SetTeamNum", &GoalWrapper::SetTeamNum).
		def("GetbNoGoalIndicator", &GoalWrapper::GetbNoGoalIndicator).
		def("SetbNoGoalIndicator", &GoalWrapper::SetbNoGoalIndicator).
		def("GetbOnlyGoalsFromDirection", &GoalWrapper::GetbOnlyGoalsFromDirection).
		def("SetbOnlyGoalsFromDirection", &GoalWrapper::SetbOnlyGoalsFromDirection).
		def("GetbShowFocusExtent", &GoalWrapper::GetbShowFocusExtent).
		def("SetbShowFocusExtent", &GoalWrapper::SetbShowFocusExtent).
		def("GetGoalDirection", &GoalWrapper::GetGoalDirection).
		def("SetGoalDirection", &GoalWrapper::SetGoalDirection).
		def("GetPointsToAward", &GoalWrapper::GetPointsToAward).
		def("SetPointsToAward", &GoalWrapper::SetPointsToAward).
		def("GetAutoCamFocusExtent", &GoalWrapper::GetAutoCamFocusExtent).
		def("SetAutoCamFocusExtent", &GoalWrapper::SetAutoCamFocusExtent).
		def("GetGoalFocusLocationOffset", &GoalWrapper::GetGoalFocusLocationOffset).
		def("SetGoalFocusLocationOffset", &GoalWrapper::SetGoalFocusLocationOffset).
		def("GetMaxGoalScorerAttachRadius", &GoalWrapper::GetMaxGoalScorerAttachRadius).
		def("SetMaxGoalScorerAttachRadius", &GoalWrapper::SetMaxGoalScorerAttachRadius).
		def("GetGoalScoredDotDirection", &GoalWrapper::GetGoalScoredDotDirection).
		def("SetGoalScoredDotDirection", &GoalWrapper::SetGoalScoredDotDirection).
		def("GetMinAttachGoalToScorerDot", &GoalWrapper::GetMinAttachGoalToScorerDot).
		def("SetMinAttachGoalToScorerDot", &GoalWrapper::SetMinAttachGoalToScorerDot).
		def("GetLocation", &GoalWrapper::GetLocation).
		def("SetLocation", &GoalWrapper::SetLocation).
		def("GetDirection", &GoalWrapper::GetDirection).
		def("SetDirection", &GoalWrapper::SetDirection).
		def("GetRotation", &GoalWrapper::GetRotation).
		def("SetRotation", &GoalWrapper::SetRotation).
		//def("GetLocalMin", &GoalWrapper::GetLocalMin).
		//def("SetLocalMin", &GoalWrapper::SetLocalMin).
		//def("GetLocalMax", &GoalWrapper::GetLocalMax).
		//def("SetLocalMax", &GoalWrapper::SetLocalMax).
		def("GetLocalExtent", &GoalWrapper::GetLocalExtent).
		def("SetLocalExtent", &GoalWrapper::SetLocalExtent).
		def("GetWorldCenter", &GoalWrapper::GetWorldCenter).
		def("SetWorldCenter", &GoalWrapper::SetWorldCenter).
		def("GetWorldExtent", &GoalWrapper::GetWorldExtent).
		def("SetWorldExtent", &GoalWrapper::SetWorldExtent);


	class_<VehicleWrapper, bases<RBActorWrapper>>("VehicleWrapper", no_init).
		def("GetVehicleSim", &VehicleWrapper::GetVehicleSim).
		def("SetVehicleSim", &VehicleWrapper::SetVehicleSim).
		def("GetbDriving", &VehicleWrapper::GetbDriving).
		def("SetbDriving", &VehicleWrapper::SetbDriving).
		def("GetbReplicatedHandbrake", &VehicleWrapper::GetbReplicatedHandbrake).
		def("SetbReplicatedHandbrake", &VehicleWrapper::SetbReplicatedHandbrake).
		def("GetbOnGround", &VehicleWrapper::GetbOnGround).
		def("SetbOnGround", &VehicleWrapper::SetbOnGround).
		def("GetbSuperSonic", &VehicleWrapper::GetbSuperSonic).
		def("SetbSuperSonic", &VehicleWrapper::SetbSuperSonic).
		def("GetbPodiumMode", &VehicleWrapper::GetbPodiumMode).
		def("SetbPodiumMode", &VehicleWrapper::SetbPodiumMode).
		def("GetInput", &VehicleWrapper::GetInput).
		def("SetInput", &VehicleWrapper::SetInput).
		def("GetReplicatedThrottle", &VehicleWrapper::GetReplicatedThrottle).
		def("SetReplicatedThrottle", &VehicleWrapper::SetReplicatedThrottle).
		def("GetReplicatedSteer", &VehicleWrapper::GetReplicatedSteer).
		def("SetReplicatedSteer", &VehicleWrapper::SetReplicatedSteer).
		def("GetPRI", &VehicleWrapper::GetPRI).
		def("SetPRI", &VehicleWrapper::SetPRI).
		def("GetVehicleUpdateTag", &VehicleWrapper::GetVehicleUpdateTag).
		def("SetVehicleUpdateTag", &VehicleWrapper::SetVehicleUpdateTag).
		def("GetLocalCollisionOffset", &VehicleWrapper::GetLocalCollisionOffset).
		def("SetLocalCollisionOffset", &VehicleWrapper::SetLocalCollisionOffset).
		def("GetLocalCollisionExtent", &VehicleWrapper::GetLocalCollisionExtent).
		def("SetLocalCollisionExtent", &VehicleWrapper::SetLocalCollisionExtent).
		def("GetLastHitBallFrame", &VehicleWrapper::GetLastHitBallFrame).
		def("SetLastHitBallFrame", &VehicleWrapper::SetLastHitBallFrame).
		def("GetBoostComponent", &VehicleWrapper::GetBoostComponent).
		def("GetDodgeComponent", &VehicleWrapper::GetDodgeComponent).
		def("GetAirControlComponent", &VehicleWrapper::GetAirControlComponent).
		def("GetJumpComponent", &VehicleWrapper::GetJumpComponent).
		def("GetTimeBelowSupersonicSpeed", &VehicleWrapper::GetTimeBelowSupersonicSpeed).
		def("SetTimeBelowSupersonicSpeed", &VehicleWrapper::SetTimeBelowSupersonicSpeed);


	class_<ReplayWrapper, bases<ObjectWrapper>>("ReplayWrapper", no_init).
		def("GetEngineVersion", &ReplayWrapper::GetEngineVersion).
		def("SetEngineVersion", &ReplayWrapper::SetEngineVersion).
		def("GetLicenseeVersion", &ReplayWrapper::GetLicenseeVersion).
		def("SetLicenseeVersion", &ReplayWrapper::SetLicenseeVersion).
		def("GetNetVersion", &ReplayWrapper::GetNetVersion).
		def("SetNetVersion", &ReplayWrapper::SetNetVersion).
		def("GetReplayVersion", &ReplayWrapper::GetReplayVersion).
		def("SetReplayVersion", &ReplayWrapper::SetReplayVersion).
		def("GetGameVersion", &ReplayWrapper::GetGameVersion).
		def("SetGameVersion", &ReplayWrapper::SetGameVersion).
		def("GetBuildID", &ReplayWrapper::GetBuildID).
		def("SetBuildID", &ReplayWrapper::SetBuildID).
		def("GetChangelist", &ReplayWrapper::GetChangelist).
		def("SetChangelist", &ReplayWrapper::SetChangelist).
		def("GetRecordFPS", &ReplayWrapper::GetRecordFPS).
		def("SetRecordFPS", &ReplayWrapper::SetRecordFPS).
		def("GetKeyframeDelay", &ReplayWrapper::GetKeyframeDelay).
		def("SetKeyframeDelay", &ReplayWrapper::SetKeyframeDelay).
		def("GetMaxChannels", &ReplayWrapper::GetMaxChannels).
		def("SetMaxChannels", &ReplayWrapper::SetMaxChannels).
		def("GetMaxReplaySizeMB", &ReplayWrapper::GetMaxReplaySizeMB).
		def("SetMaxReplaySizeMB", &ReplayWrapper::SetMaxReplaySizeMB).
		def("GetNumFrames", &ReplayWrapper::GetNumFrames).
		def("SetNumFrames", &ReplayWrapper::SetNumFrames).
		def("GetbFileCorrupted", &ReplayWrapper::GetbFileCorrupted).
		def("SetbFileCorrupted", &ReplayWrapper::SetbFileCorrupted).
		def("GetbForceKeyframe", &ReplayWrapper::GetbForceKeyframe).
		def("SetbForceKeyframe", &ReplayWrapper::SetbForceKeyframe).
		def("GetbLoadedNetPackages", &ReplayWrapper::GetbLoadedNetPackages).
		def("SetbLoadedNetPackages", &ReplayWrapper::SetbLoadedNetPackages).
		def("GetbDebug", &ReplayWrapper::GetbDebug).
		def("SetbDebug", &ReplayWrapper::SetbDebug).
		def("GetReplayState", &ReplayWrapper::GetReplayState).
		def("SetReplayState", &ReplayWrapper::SetReplayState).
		def("GetCurrentFrame", &ReplayWrapper::GetCurrentFrame).
		def("SetCurrentFrame", &ReplayWrapper::SetCurrentFrame).
		def("GetNextKeyframe", &ReplayWrapper::GetNextKeyframe).
		def("SetNextKeyframe", &ReplayWrapper::SetNextKeyframe).
		//def("GetCurrentTime", &ReplayWrapper::GetCurrentTime).
		def("SetCurrentTime", &ReplayWrapper::SetCurrentTime).
		def("GetAccumulatedDeltaTime", &ReplayWrapper::GetAccumulatedDeltaTime).
		def("SetAccumulatedDeltaTime", &ReplayWrapper::SetAccumulatedDeltaTime).
		def("GetTimeToSkipTo", &ReplayWrapper::GetTimeToSkipTo).
		def("SetTimeToSkipTo", &ReplayWrapper::SetTimeToSkipTo).
		def("GetFrameToSkipTo", &ReplayWrapper::GetFrameToSkipTo).
		def("SetFrameToSkipTo", &ReplayWrapper::SetFrameToSkipTo).
		def("GetPlayersOnlyTicks", &ReplayWrapper::GetPlayersOnlyTicks).
		def("SetPlayersOnlyTicks", &ReplayWrapper::SetPlayersOnlyTicks);


	class_<TeamWrapper, bases<ActorWrapper>>("TeamWrapper", no_init).
		def("GetFontColor", &TeamWrapper::GetFontColor).
		def("SetFontColor", &TeamWrapper::SetFontColor).
		def("GetColorBlindFontColor", &TeamWrapper::GetColorBlindFontColor).
		def("SetColorBlindFontColor", &TeamWrapper::SetColorBlindFontColor).
		def("GetGameEvent", &TeamWrapper::GetGameEvent).
		def("SetGameEvent", &TeamWrapper::SetGameEvent).
		def("GetMembers", &TeamWrapper::GetMembers).
		def("GetbForfeit", &TeamWrapper::GetbForfeit).
		def("SetbForfeit", &TeamWrapper::SetbForfeit);


	class_<VehicleSimWrapper, bases<ObjectWrapper>>("VehicleSimWrapper", no_init).
		def("GetWheels", &VehicleSimWrapper::GetWheels).
		def("GetDriveTorque", &VehicleSimWrapper::GetDriveTorque).
		def("SetDriveTorque", &VehicleSimWrapper::SetDriveTorque).
		def("GetBrakeTorque", &VehicleSimWrapper::GetBrakeTorque).
		def("SetBrakeTorque", &VehicleSimWrapper::SetBrakeTorque).
		def("GetStopThreshold", &VehicleSimWrapper::GetStopThreshold).
		def("SetStopThreshold", &VehicleSimWrapper::SetStopThreshold).
		def("GetIdleBrakeFactor", &VehicleSimWrapper::GetIdleBrakeFactor).
		def("SetIdleBrakeFactor", &VehicleSimWrapper::SetIdleBrakeFactor).
		def("GetOppositeBrakeFactor", &VehicleSimWrapper::GetOppositeBrakeFactor).
		def("SetOppositeBrakeFactor", &VehicleSimWrapper::SetOppositeBrakeFactor).
		def("GetbUseAckermannSteering", &VehicleSimWrapper::GetbUseAckermannSteering).
		def("SetbUseAckermannSteering", &VehicleSimWrapper::SetbUseAckermannSteering).
		def("GetbWasAttached", &VehicleSimWrapper::GetbWasAttached).
		def("SetbWasAttached", &VehicleSimWrapper::SetbWasAttached).
		def("GetOutputThrottle", &VehicleSimWrapper::GetOutputThrottle).
		def("SetOutputThrottle", &VehicleSimWrapper::SetOutputThrottle).
		def("GetOutputSteer", &VehicleSimWrapper::GetOutputSteer).
		def("SetOutputSteer", &VehicleSimWrapper::SetOutputSteer).
		def("GetOutputBrake", &VehicleSimWrapper::GetOutputBrake).
		def("SetOutputBrake", &VehicleSimWrapper::SetOutputBrake).
		def("GetOutputHandbrake", &VehicleSimWrapper::GetOutputHandbrake).
		def("SetOutputHandbrake", &VehicleSimWrapper::SetOutputHandbrake).
		def("GetVehicle", &VehicleSimWrapper::GetVehicle).
		def("SetVehicle", &VehicleSimWrapper::SetVehicle).
		def("GetCar", &VehicleSimWrapper::GetCar).
		def("SetCar", &VehicleSimWrapper::SetCar).
		def("GetSteeringSensitivity", &VehicleSimWrapper::GetSteeringSensitivity).
		def("SetSteeringSensitivity", &VehicleSimWrapper::SetSteeringSensitivity);


	class_<WheelWrapper, bases<ObjectWrapper>>("WheelWrapper", no_init).
		def("GetSteerFactor", &WheelWrapper::GetSteerFactor).
		def("SetSteerFactor", &WheelWrapper::SetSteerFactor).
		def("GetWheelRadius", &WheelWrapper::GetWheelRadius).
		def("SetWheelRadius", &WheelWrapper::SetWheelRadius).
		def("GetSuspensionStiffness", &WheelWrapper::GetSuspensionStiffness).
		def("SetSuspensionStiffness", &WheelWrapper::SetSuspensionStiffness).
		def("GetSuspensionDampingCompression", &WheelWrapper::GetSuspensionDampingCompression).
		def("SetSuspensionDampingCompression", &WheelWrapper::SetSuspensionDampingCompression).
		def("GetSuspensionDampingRelaxation", &WheelWrapper::GetSuspensionDampingRelaxation).
		def("SetSuspensionDampingRelaxation", &WheelWrapper::SetSuspensionDampingRelaxation).
		def("GetSuspensionTravel", &WheelWrapper::GetSuspensionTravel).
		def("SetSuspensionTravel", &WheelWrapper::SetSuspensionTravel).
		def("GetSuspensionMaxRaise", &WheelWrapper::GetSuspensionMaxRaise).
		def("SetSuspensionMaxRaise", &WheelWrapper::SetSuspensionMaxRaise).
		def("GetContactForceDistance", &WheelWrapper::GetContactForceDistance).
		def("SetContactForceDistance", &WheelWrapper::SetContactForceDistance).
		def("GetSpinSpeedDecayRate", &WheelWrapper::GetSpinSpeedDecayRate).
		def("SetSpinSpeedDecayRate", &WheelWrapper::SetSpinSpeedDecayRate).
		def("GetBoneOffset", &WheelWrapper::GetBoneOffset).
		def("SetBoneOffset", &WheelWrapper::SetBoneOffset).
		def("GetPresetRestPosition", &WheelWrapper::GetPresetRestPosition).
		def("SetPresetRestPosition", &WheelWrapper::SetPresetRestPosition).
		def("GetLocalSuspensionRayStart", &WheelWrapper::GetLocalSuspensionRayStart).
		def("SetLocalSuspensionRayStart", &WheelWrapper::SetLocalSuspensionRayStart).
		def("GetLocalRestPosition", &WheelWrapper::GetLocalRestPosition).
		def("SetLocalRestPosition", &WheelWrapper::SetLocalRestPosition).
		def("GetVehicleSim", &WheelWrapper::GetVehicleSim).
		def("SetVehicleSim", &WheelWrapper::SetVehicleSim).
		def("GetWheelIndex", &WheelWrapper::GetWheelIndex).
		def("SetWheelIndex", &WheelWrapper::SetWheelIndex).
		def("GetbDrawDebug", &WheelWrapper::GetbDrawDebug).
		def("SetbDrawDebug", &WheelWrapper::SetbDrawDebug).
		def("GetbHadContact", &WheelWrapper::GetbHadContact).
		def("SetbHadContact", &WheelWrapper::SetbHadContact).
		def("GetFrictionCurveInput", &WheelWrapper::GetFrictionCurveInput).
		def("SetFrictionCurveInput", &WheelWrapper::SetFrictionCurveInput).
		def("GetSpinSpeed", &WheelWrapper::GetSpinSpeed).
		def("SetSpinSpeed", &WheelWrapper::SetSpinSpeed);


	class_<PriWrapper, bases<PlayerReplicationInfoWrapper>>("PriWrapper", no_init).
		def("GetMatchScore", &PriWrapper::GetMatchScore).
		def("SetMatchScore", &PriWrapper::SetMatchScore).
		def("GetMatchGoals", &PriWrapper::GetMatchGoals).
		def("SetMatchGoals", &PriWrapper::SetMatchGoals).
		def("GetMatchOwnGoals", &PriWrapper::GetMatchOwnGoals).
		def("SetMatchOwnGoals", &PriWrapper::SetMatchOwnGoals).
		def("GetMatchAssists", &PriWrapper::GetMatchAssists).
		def("SetMatchAssists", &PriWrapper::SetMatchAssists).
		def("GetMatchSaves", &PriWrapper::GetMatchSaves).
		def("SetMatchSaves", &PriWrapper::SetMatchSaves).
		def("GetMatchShots", &PriWrapper::GetMatchShots).
		def("SetMatchShots", &PriWrapper::SetMatchShots).
		def("GetMatchDemolishes", &PriWrapper::GetMatchDemolishes).
		def("SetMatchDemolishes", &PriWrapper::SetMatchDemolishes).
		def("GetMatchBonusXP", &PriWrapper::GetMatchBonusXP).
		def("SetMatchBonusXP", &PriWrapper::SetMatchBonusXP).
		def("GetMatchBreakoutDamage", &PriWrapper::GetMatchBreakoutDamage).
		def("SetMatchBreakoutDamage", &PriWrapper::SetMatchBreakoutDamage).
		def("GetbMatchMVP", &PriWrapper::GetbMatchMVP).
		def("SetbMatchMVP", &PriWrapper::SetbMatchMVP).
		def("GetbMatchAdmin", &PriWrapper::GetbMatchAdmin).
		def("SetbMatchAdmin", &PriWrapper::SetbMatchAdmin).
		def("GetbLoadoutSet", &PriWrapper::GetbLoadoutSet).
		def("SetbLoadoutSet", &PriWrapper::SetbLoadoutSet).
		def("GetbOnlineLoadoutSet", &PriWrapper::GetbOnlineLoadoutSet).
		def("SetbOnlineLoadoutSet", &PriWrapper::SetbOnlineLoadoutSet).
		def("GetbLoadoutsSet", &PriWrapper::GetbLoadoutsSet).
		def("SetbLoadoutsSet", &PriWrapper::SetbLoadoutsSet).
		def("GetbOnlineLoadoutsSet", &PriWrapper::GetbOnlineLoadoutsSet).
		def("SetbOnlineLoadoutsSet", &PriWrapper::SetbOnlineLoadoutsSet).
		def("GetbTeamPaintSet", &PriWrapper::GetbTeamPaintSet).
		def("SetbTeamPaintSet", &PriWrapper::SetbTeamPaintSet).
		def("GetbReady", &PriWrapper::GetbReady).
		def("SetbReady", &PriWrapper::SetbReady).
		def("GetbBusy", &PriWrapper::GetbBusy).
		def("SetbBusy", &PriWrapper::SetbBusy).
		def("GetbUsingSecondaryCamera", &PriWrapper::GetbUsingSecondaryCamera).
		def("SetbUsingSecondaryCamera", &PriWrapper::SetbUsingSecondaryCamera).
		def("GetbUsingBehindView", &PriWrapper::GetbUsingBehindView).
		def("SetbUsingBehindView", &PriWrapper::SetbUsingBehindView).
		def("GetbUsingFreecam", &PriWrapper::GetbUsingFreecam).
		def("SetbUsingFreecam", &PriWrapper::SetbUsingFreecam).
		def("GetbIsInSplitScreen", &PriWrapper::GetbIsInSplitScreen).
		def("SetbIsInSplitScreen", &PriWrapper::SetbIsInSplitScreen).
		def("GetbDeveloper", &PriWrapper::GetbDeveloper).
		def("SetbDeveloper", &PriWrapper::SetbDeveloper).
		def("GetbVoteToForfeitDisabled", &PriWrapper::GetbVoteToForfeitDisabled).
		def("SetbVoteToForfeitDisabled", &PriWrapper::SetbVoteToForfeitDisabled).
		def("GetbUsingItems", &PriWrapper::GetbUsingItems).
		def("SetbUsingItems", &PriWrapper::SetbUsingItems).
		def("GetPlayerHistoryValid", &PriWrapper::GetPlayerHistoryValid).
		def("SetPlayerHistoryValid", &PriWrapper::SetPlayerHistoryValid).
		def("GetGameEvent", &PriWrapper::GetGameEvent).
		def("SetGameEvent", &PriWrapper::SetGameEvent).
		def("GetReplicatedGameEvent", &PriWrapper::GetReplicatedGameEvent).
		def("SetReplicatedGameEvent", &PriWrapper::SetReplicatedGameEvent).
		def("GetCar", &PriWrapper::GetCar).
		def("SetCar", &PriWrapper::SetCar).
		def("GetRespawnTimeRemaining", &PriWrapper::GetRespawnTimeRemaining).
		def("SetRespawnTimeRemaining", &PriWrapper::SetRespawnTimeRemaining).
		def("GetWaitingStartTime", &PriWrapper::GetWaitingStartTime).
		def("SetWaitingStartTime", &PriWrapper::SetWaitingStartTime).
		def("GetTotalGameTimePlayed", &PriWrapper::GetTotalGameTimePlayed).
		def("SetTotalGameTimePlayed", &PriWrapper::SetTotalGameTimePlayed).
		def("GetCameraPitch", &PriWrapper::GetCameraPitch).
		def("SetCameraPitch", &PriWrapper::SetCameraPitch).
		def("GetCameraYaw", &PriWrapper::GetCameraYaw).
		def("SetCameraYaw", &PriWrapper::SetCameraYaw).
		def("GetPawnType", &PriWrapper::GetPawnType).
		def("SetPawnType", &PriWrapper::SetPawnType).
		def("GetReplicatedWorstNetQualityBeyondLatency", &PriWrapper::GetReplicatedWorstNetQualityBeyondLatency).
		def("SetReplicatedWorstNetQualityBeyondLatency", &PriWrapper::SetReplicatedWorstNetQualityBeyondLatency).
		def("GetTotalXP", &PriWrapper::GetTotalXP).
		def("SetTotalXP", &PriWrapper::SetTotalXP).
		def("GetDodgeInputThreshold", &PriWrapper::GetDodgeInputThreshold).
		def("SetDodgeInputThreshold", &PriWrapper::SetDodgeInputThreshold).
		def("GetSteeringSensitivity", &PriWrapper::GetSteeringSensitivity).
		def("SetSteeringSensitivity", &PriWrapper::SetSteeringSensitivity).
		def("GetAirControlSensitivity", &PriWrapper::GetAirControlSensitivity).
		def("SetAirControlSensitivity", &PriWrapper::SetAirControlSensitivity).
		def("GetNextTimeRestrictedStatEventAllowedTime", &PriWrapper::GetNextTimeRestrictedStatEventAllowedTime).
		def("SetNextTimeRestrictedStatEventAllowedTime", &PriWrapper::SetNextTimeRestrictedStatEventAllowedTime).
		def("GetLastTimeRestrictedStatEventTime", &PriWrapper::GetLastTimeRestrictedStatEventTime).
		def("SetLastTimeRestrictedStatEventTime", &PriWrapper::SetLastTimeRestrictedStatEventTime).
		def("GetTimeTillItem", &PriWrapper::GetTimeTillItem).
		def("SetTimeTillItem", &PriWrapper::SetTimeTillItem).
		def("GetMaxTimeTillItem", &PriWrapper::GetMaxTimeTillItem).
		def("SetMaxTimeTillItem", &PriWrapper::SetMaxTimeTillItem).
		def("GetBoostPickups", &PriWrapper::GetBoostPickups).
		def("SetBoostPickups", &PriWrapper::SetBoostPickups).
		def("GetBallTouches", &PriWrapper::GetBallTouches).
		def("SetBallTouches", &PriWrapper::SetBallTouches).
		def("GetCarTouches", &PriWrapper::GetCarTouches).
		def("SetCarTouches", &PriWrapper::SetCarTouches).
		def("GetReplacingBotPRI", &PriWrapper::GetReplacingBotPRI).
		def("SetReplacingBotPRI", &PriWrapper::SetReplacingBotPRI);


	class_<CarWrapper, bases<VehicleWrapper>>("CarWrapper", no_init).
		def("GetDefaultCarComponents", &CarWrapper::GetDefaultCarComponents).
		def("GetDoubleJumpComponent", &CarWrapper::GetDoubleJumpComponent).
		def("GetFlipComponent", &CarWrapper::GetFlipComponent).
		def("GetDemolishTarget", &CarWrapper::GetDemolishTarget).
		def("SetDemolishTarget", &CarWrapper::SetDemolishTarget).
		def("GetDemolishSpeed", &CarWrapper::GetDemolishSpeed).
		def("SetDemolishSpeed", &CarWrapper::SetDemolishSpeed).
		def("GetMaxTimeForDodge", &CarWrapper::GetMaxTimeForDodge).
		def("SetMaxTimeForDodge", &CarWrapper::SetMaxTimeForDodge).
		def("GetLastWheelsHitBallTime", &CarWrapper::GetLastWheelsHitBallTime).
		def("SetLastWheelsHitBallTime", &CarWrapper::SetLastWheelsHitBallTime).
		def("GetReplicatedCarScale", &CarWrapper::GetReplicatedCarScale).
		def("SetReplicatedCarScale", &CarWrapper::SetReplicatedCarScale).
		def("GetbDemolishOnOpposingGround", &CarWrapper::GetbDemolishOnOpposingGround).
		def("SetbDemolishOnOpposingGround", &CarWrapper::SetbDemolishOnOpposingGround).
		def("GetbWasOnOpposingGround", &CarWrapper::GetbWasOnOpposingGround).
		def("SetbWasOnOpposingGround", &CarWrapper::SetbWasOnOpposingGround).
		def("GetbDemolishOnGoalZone", &CarWrapper::GetbDemolishOnGoalZone).
		def("SetbDemolishOnGoalZone", &CarWrapper::SetbDemolishOnGoalZone).
		def("GetbWasInGoalZone", &CarWrapper::GetbWasInGoalZone).
		def("SetbWasInGoalZone", &CarWrapper::SetbWasInGoalZone).
		def("GetbJumped", &CarWrapper::GetbJumped).
		def("SetbJumped", &CarWrapper::SetbJumped).
		def("GetbDoubleJumped", &CarWrapper::GetbDoubleJumped).
		def("SetbDoubleJumped", &CarWrapper::SetbDoubleJumped).
		def("GetbOverrideHandbrakeOn", &CarWrapper::GetbOverrideHandbrakeOn).
		def("SetbOverrideHandbrakeOn", &CarWrapper::SetbOverrideHandbrakeOn).
		def("GetbOverrideBoostOn", &CarWrapper::GetbOverrideBoostOn).
		def("SetbOverrideBoostOn", &CarWrapper::SetbOverrideBoostOn).
		def("GetAttackerPRI", &CarWrapper::GetAttackerPRI).
		def("SetAttackerPRI", &CarWrapper::SetAttackerPRI).
		def("GetAttachedBall", &CarWrapper::GetAttachedBall).
		def("SetAttachedBall", &CarWrapper::SetAttachedBall).
		def("GetMouseAccel", &CarWrapper::GetMouseAccel).
		def("SetMouseAccel", &CarWrapper::SetMouseAccel).
		def("GetMouseAirAccel", &CarWrapper::GetMouseAirAccel).
		def("SetMouseAirAccel", &CarWrapper::SetMouseAirAccel).
		def("GetAttachedPickup", &CarWrapper::GetAttachedPickup).
		def("SetAttachedPickup", &CarWrapper::SetAttachedPickup).
		def("GetReplayFocusOffset", &CarWrapper::GetReplayFocusOffset).
		def("SetReplayFocusOffset", &CarWrapper::SetReplayFocusOffset).
		def("GetAddedBallForceMultiplier", &CarWrapper::GetAddedBallForceMultiplier).
		def("SetAddedBallForceMultiplier", &CarWrapper::SetAddedBallForceMultiplier).
		def("GetAddedCarForceMultiplier", &CarWrapper::GetAddedCarForceMultiplier).
		def("SetAddedCarForceMultiplier", &CarWrapper::SetAddedCarForceMultiplier).
		def("GetGameEvent", &CarWrapper::GetGameEvent).
		def("SetGameEvent", &CarWrapper::SetGameEvent);


	class_<ServerWrapper, bases<TeamGameEventWrapper>>("ServerWrapper", no_init).
		def("GetBall", &ServerWrapper::GetBall).
		//	def("is_training", &ServerWrapper::IsTraining).
		//	def("is_in_free_play", &GameEventWrapper::IsInFreePlay).
		//	def("is_server", &GameEventWrapper::IsServer).
		//	def("kick_bots", &GameEventWrapper::KickBots).
		//	def("get_pri_car", &GameEventWrapper::GetPRICar);
		def("GetTestCarArchetype", &ServerWrapper::GetTestCarArchetype).
		def("SetTestCarArchetype", &ServerWrapper::SetTestCarArchetype).
		def("GetBallArchetype", &ServerWrapper::GetBallArchetype).
		def("SetBallArchetype", &ServerWrapper::SetBallArchetype).
		def("GetBallSpawnPoint", &ServerWrapper::GetBallSpawnPoint).
		def("SetBallSpawnPoint", &ServerWrapper::SetBallSpawnPoint).
		def("GetSeriesLength", &ServerWrapper::GetSeriesLength).
		def("SetSeriesLength", &ServerWrapper::SetSeriesLength).
		def("GetGameTime", &ServerWrapper::GetGameTime).
		def("SetGameTime", &ServerWrapper::SetGameTime).
		def("GetWarmupTime", &ServerWrapper::GetWarmupTime).
		def("SetWarmupTime", &ServerWrapper::SetWarmupTime).
		def("GetMaxScore", &ServerWrapper::GetMaxScore).
		def("SetMaxScore", &ServerWrapper::SetMaxScore).
		def("GetAutoBalanceDifference", &ServerWrapper::GetAutoBalanceDifference).
		def("SetAutoBalanceDifference", &ServerWrapper::SetAutoBalanceDifference).
		def("GetLastTrialTime", &ServerWrapper::GetLastTrialTime).
		def("SetLastTrialTime", &ServerWrapper::SetLastTrialTime).
		def("GetScoreSlomoTime", &ServerWrapper::GetScoreSlomoTime).
		def("SetScoreSlomoTime", &ServerWrapper::SetScoreSlomoTime).
		def("GetGameTimeRemaining", &ServerWrapper::GetGameTimeRemaining).
		def("SetGameTimeRemaining", &ServerWrapper::SetGameTimeRemaining).
		def("GetSecondsRemaining", &ServerWrapper::GetSecondsRemaining).
		def("SetSecondsRemaining", &ServerWrapper::SetSecondsRemaining).
		def("GetTotalGameTimePlayed", &ServerWrapper::GetTotalGameTimePlayed).
		def("SetTotalGameTimePlayed", &ServerWrapper::SetTotalGameTimePlayed).
		def("GetbRoundActive", &ServerWrapper::GetbRoundActive).
		def("SetbRoundActive", &ServerWrapper::SetbRoundActive).
		def("GetbPlayReplays", &ServerWrapper::GetbPlayReplays).
		def("SetbPlayReplays", &ServerWrapper::SetbPlayReplays).
		def("GetbBallHasBeenHit", &ServerWrapper::GetbBallHasBeenHit).
		def("SetbBallHasBeenHit", &ServerWrapper::SetbBallHasBeenHit).
		def("GetbOverTime", &ServerWrapper::GetbOverTime).
		def("SetbOverTime", &ServerWrapper::SetbOverTime).
		def("GetbUnlimitedTime", &ServerWrapper::GetbUnlimitedTime).
		def("SetbUnlimitedTime", &ServerWrapper::SetbUnlimitedTime).
		def("GetbKickOnTrialEnd", &ServerWrapper::GetbKickOnTrialEnd).
		def("SetbKickOnTrialEnd", &ServerWrapper::SetbKickOnTrialEnd).
		def("GetbNoContest", &ServerWrapper::GetbNoContest).
		def("SetbNoContest", &ServerWrapper::SetbNoContest).
		def("GetbDisableGoalDelay", &ServerWrapper::GetbDisableGoalDelay).
		def("SetbDisableGoalDelay", &ServerWrapper::SetbDisableGoalDelay).
		//def("GetbDisableSpawnInRedZone", &ServerWrapper::GetbDisableSpawnInRedZone).
		//def("SetbDisableSpawnInRedZone", &ServerWrapper::SetbDisableSpawnInRedZone).
		def("GetbShowNoScorerGoalMessage", &ServerWrapper::GetbShowNoScorerGoalMessage).
		def("SetbShowNoScorerGoalMessage", &ServerWrapper::SetbShowNoScorerGoalMessage).
		def("GetbMatchEnded", &ServerWrapper::GetbMatchEnded).
		def("SetbMatchEnded", &ServerWrapper::SetbMatchEnded).
		def("GetbShowIntroScene", &ServerWrapper::GetbShowIntroScene).
		def("SetbShowIntroScene", &ServerWrapper::SetbShowIntroScene).
		def("GetbClubMatch", &ServerWrapper::GetbClubMatch).
		def("SetbClubMatch", &ServerWrapper::SetbClubMatch).
		def("GetNextSpawnIndex", &ServerWrapper::GetNextSpawnIndex).
		def("SetNextSpawnIndex", &ServerWrapper::SetNextSpawnIndex).
		def("GetReplayDirectorArchetype", &ServerWrapper::GetReplayDirectorArchetype).
		def("SetReplayDirectorArchetype", &ServerWrapper::SetReplayDirectorArchetype).
		def("GetReplayDirector", &ServerWrapper::GetReplayDirector).
		def("SetReplayDirector", &ServerWrapper::SetReplayDirector).
		def("GetGameBalls", &ServerWrapper::GetGameBalls).
		def("GetTotalGameBalls", &ServerWrapper::GetTotalGameBalls).
		def("SetTotalGameBalls", &ServerWrapper::SetTotalGameBalls).
		def("GetPostGoalTime", &ServerWrapper::GetPostGoalTime).
		def("SetPostGoalTime", &ServerWrapper::SetPostGoalTime).
		def("GetGoals", &ServerWrapper::GetGoals).
		def("GetSecondsRemainingCountdown", &ServerWrapper::GetSecondsRemainingCountdown).
		def("SetSecondsRemainingCountdown", &ServerWrapper::SetSecondsRemainingCountdown).
		def("GetFieldCenter", &ServerWrapper::GetFieldCenter).
		def("SetFieldCenter", &ServerWrapper::SetFieldCenter).
		def("GetGameWinner", &ServerWrapper::GetGameWinner).
		def("SetGameWinner", &ServerWrapper::SetGameWinner).
		def("GetMatchWinner", &ServerWrapper::GetMatchWinner).
		def("SetMatchWinner", &ServerWrapper::SetMatchWinner).
		def("GetMVP", &ServerWrapper::GetMVP).
		def("SetMVP", &ServerWrapper::SetMVP).
		def("GetFastestGoalPlayer", &ServerWrapper::GetFastestGoalPlayer).
		def("SetFastestGoalPlayer", &ServerWrapper::SetFastestGoalPlayer).
		def("GetSlowestGoalPlayer", &ServerWrapper::GetSlowestGoalPlayer).
		def("SetSlowestGoalPlayer", &ServerWrapper::SetSlowestGoalPlayer).
		def("GetFurthestGoalPlayer", &ServerWrapper::GetFurthestGoalPlayer).
		def("SetFurthestGoalPlayer", &ServerWrapper::SetFurthestGoalPlayer).
		def("GetFastestGoalSpeed", &ServerWrapper::GetFastestGoalSpeed).
		def("SetFastestGoalSpeed", &ServerWrapper::SetFastestGoalSpeed).
		def("GetSlowestGoalSpeed", &ServerWrapper::GetSlowestGoalSpeed).
		def("SetSlowestGoalSpeed", &ServerWrapper::SetSlowestGoalSpeed).
		def("GetFurthestGoal", &ServerWrapper::GetFurthestGoal).
		def("SetFurthestGoal", &ServerWrapper::SetFurthestGoal).
		def("GetReplicatedScoredOnTeam", &ServerWrapper::GetReplicatedScoredOnTeam).
		def("SetReplicatedScoredOnTeam", &ServerWrapper::SetReplicatedScoredOnTeam).
		def("GetReplicatedServerPerformanceState", &ServerWrapper::GetReplicatedServerPerformanceState).
		def("SetReplicatedServerPerformanceState", &ServerWrapper::SetReplicatedServerPerformanceState).
		def("GetRoundNum", &ServerWrapper::GetRoundNum).
		def("SetRoundNum", &ServerWrapper::SetRoundNum).
		def("GetKickIdleReplayOffset", &ServerWrapper::GetKickIdleReplayOffset).
		def("SetKickIdleReplayOffset", &ServerWrapper::SetKickIdleReplayOffset).
		def("GetAssistMaxTime", &ServerWrapper::GetAssistMaxTime).
		def("SetAssistMaxTime", &ServerWrapper::SetAssistMaxTime).
		def("GetBallHasBeenHitStartDelay", &ServerWrapper::GetBallHasBeenHitStartDelay).
		def("SetBallHasBeenHitStartDelay", &ServerWrapper::SetBallHasBeenHitStartDelay).
		//def("GetLowFPSRate", &ServerWrapper::GetLowFPSRate).
		//def("SetLowFPSRate", &ServerWrapper::SetLowFPSRate).
		//def("GetLowFPSTimeThreshold", &ServerWrapper::GetLowFPSTimeThreshold).
		//def("SetLowFPSTimeThreshold", &ServerWrapper::SetLowFPSTimeThreshold).
		//def("GetLowFPSTime", &ServerWrapper::GetLowFPSTime).
		//def("SetLowFPSTime", &ServerWrapper::SetLowFPSTime).
		def("GetPodiumDelay", &ServerWrapper::GetPodiumDelay).
		def("SetPodiumDelay", &ServerWrapper::SetPodiumDelay).
		def("GetPodiumTime", &ServerWrapper::GetPodiumTime).
		def("SetPodiumTime", &ServerWrapper::SetPodiumTime).
		def("GetLobbyEndCountdown", &ServerWrapper::GetLobbyEndCountdown).
		def("SetLobbyEndCountdown", &ServerWrapper::SetLobbyEndCountdown).
		def("GetLobbyCountdown", &ServerWrapper::GetLobbyCountdown).
		def("SetLobbyCountdown", &ServerWrapper::SetLobbyCountdown).
		def("GetLobbyTime", &ServerWrapper::GetLobbyTime).
		def("SetLobbyTime", &ServerWrapper::SetLobbyTime).
		def("GetLobbySpawnRestartTime", &ServerWrapper::GetLobbySpawnRestartTime).
		def("SetLobbySpawnRestartTime", &ServerWrapper::SetLobbySpawnRestartTime);



	class_<TutorialWrapper, bases<ServerWrapper>>("TutorialWrapper", no_init).
		def("get_team_num", &TutorialWrapper::GetTeamNum).
		def("set_team_num", &TutorialWrapper::SetTeamNum).
		def("get_redo_count", &TutorialWrapper::GetRedoCount).
		def("set_redo_count", &TutorialWrapper::SetRedoCount).
		def("get_redo_total", &TutorialWrapper::GetRedoTotal).
		def("set_redo_total", &TutorialWrapper::SetRedoTotal).
		//def("get_countdown_time_left", &TutorialWrapper::GetCountdownTimeLeft).
		//def("set_countdown_time_left", &TutorialWrapper::SetCountdownTimeLeft).
		def("get_ball_goal_num", &TutorialWrapper::GetBallGoalNum).
		def("set_ball_goal_num", &TutorialWrapper::SetBallGoalNum).
		//def("get_only_score_in_ball_goal_num", &TutorialWrapper::GetOnlyScoreInBallGoalNum).
		//def("set_only_score_in_ball_goal_num", &TutorialWrapper::SetOnlyScoreInBallGoalNum).
		//def("get_is_unlimited_boost", &TutorialWrapper::GetIsUnlimitedBoost).
		//def("set_is_unlimited_boost", &TutorialWrapper::SetIsUnlimitedBoost).
		//def("get_show_boost_meter", &TutorialWrapper::GetShowBoostMeter).
		def("SetShowBoostMeter", &TutorialWrapper::SetShowBoostMeter).
		def("GetBallBounceScale", &TutorialWrapper::GetBallBounceScale).
		def("SetBallBounceScale", &TutorialWrapper::SetBallBounceScale).
		def("GetBallSpawnLocation", &TutorialWrapper::GetBallSpawnLocation).
		def("SetBallSpawnLocation", &TutorialWrapper::SetBallSpawnLocation).
		//def("get_ball_start_velocity", &TutorialWrapper::GetBallStartVelocity).
		//def("set_ball_start_velocity", &TutorialWrapper::SetBallStartVelocity).
		def("get_total_field_extent", &TutorialWrapper::GetTotalFieldExtent).
		def("set_total_field_extent", &TutorialWrapper::SetTotalFieldExtent).
		def("redo", &TutorialWrapper::Redo).
		def("get_car_spawn_location", &TutorialWrapper::GetCarSpawnLocation).
		def("set_car_spawn_location", &TutorialWrapper::SetCarSpawnLocation).
		def("get_car_spawn_rotation", &TutorialWrapper::GetCarSpawnRotation).
		def("set_car_spawn_rotation", &TutorialWrapper::SetCarSpawnRotation).
		def("GetGameCar", &TutorialWrapper::GetGameCar).
		def("IsBallMovingTowardsGoal", &TutorialWrapper::IsBallMovingTowardsGoal).
		def("IsInGoal", &TutorialWrapper::IsInGoal).
		def("DisableGoalReset", &TutorialWrapper::DisableGoalReset).
		def("EnableGoalReset", &TutorialWrapper::EnableGoalReset).
		//def("spawn_car", &TutorialWrapper::SpawnCar).
		def("GenerateShot", &TutorialWrapper::GenerateShot).
		def("GenerateGoalAimLocation", &TutorialWrapper::GenerateGoalAimLocation).
		def("GetGoalExtent", &TutorialWrapper::GetGoalExtent).
		def("GetTotalFieldExtent", &TutorialWrapper::GetTotalFieldExtent).
		def("SetTotalFieldExtent", &TutorialWrapper::SetTotalFieldExtent).
		def("GetTeamNum", &TutorialWrapper::GetTeamNum).
		def("SetTeamNum", &TutorialWrapper::SetTeamNum).
		def("GetBallGoalNum", &TutorialWrapper::GetBallGoalNum).
		def("SetBallGoalNum", &TutorialWrapper::SetBallGoalNum).
		def("GetbOnlyScoreInBallGoalNum", &TutorialWrapper::GetbOnlyScoreInBallGoalNum).
		def("SetbOnlyScoreInBallGoalNum", &TutorialWrapper::SetbOnlyScoreInBallGoalNum).
		def("GetbRedoRound", &TutorialWrapper::GetbRedoRound).
		def("SetbRedoRound", &TutorialWrapper::SetbRedoRound).
		def("GetbAllowSuperBoost", &TutorialWrapper::GetbAllowSuperBoost).
		def("SetbAllowSuperBoost", &TutorialWrapper::SetbAllowSuperBoost).
		def("GetbDisplayedRedoPenaltyMessage", &TutorialWrapper::GetbDisplayedRedoPenaltyMessage).
		def("SetbDisplayedRedoPenaltyMessage", &TutorialWrapper::SetbDisplayedRedoPenaltyMessage).
		def("GetbShowBoostMeter", &TutorialWrapper::GetbShowBoostMeter).
		def("SetbShowBoostMeter", &TutorialWrapper::SetbShowBoostMeter).
		def("GetDifficulty", &TutorialWrapper::GetDifficulty).
		def("SetDifficulty", &TutorialWrapper::SetDifficulty).
		def("GetDebugRotationType", &TutorialWrapper::GetDebugRotationType).
		def("SetDebugRotationType", &TutorialWrapper::SetDebugRotationType).
		def("GetGoalDepth", &TutorialWrapper::GetGoalDepth).
		def("SetGoalDepth", &TutorialWrapper::SetGoalDepth).
		def("GetGameEventRounds", &TutorialWrapper::GetGameEventRounds).
		def("SetGameEventRounds", &TutorialWrapper::SetGameEventRounds).
		def("GetEventStartTime", &TutorialWrapper::GetEventStartTime).
		def("SetEventStartTime", &TutorialWrapper::SetEventStartTime).
		def("GetBallInitialVelocity", &TutorialWrapper::GetBallInitialVelocity).
		def("SetBallInitialVelocity", &TutorialWrapper::SetBallInitialVelocity).
		def("GetSpawnIndexTypeOverride", &TutorialWrapper::GetSpawnIndexTypeOverride).
		def("SetSpawnIndexTypeOverride", &TutorialWrapper::SetSpawnIndexTypeOverride).
		def("GetWaveIndex", &TutorialWrapper::GetWaveIndex).
		def("SetWaveIndex", &TutorialWrapper::SetWaveIndex).
		def("GetWaveSpawnCount", &TutorialWrapper::GetWaveSpawnCount).
		def("SetWaveSpawnCount", &TutorialWrapper::SetWaveSpawnCount).
		def("GetRandomSpawnIndex", &TutorialWrapper::GetRandomSpawnIndex).
		def("SetRandomSpawnIndex", &TutorialWrapper::SetRandomSpawnIndex).
		def("GetBallSpawnLocation", &TutorialWrapper::GetBallSpawnLocation).
		def("SetBallSpawnLocation", &TutorialWrapper::SetBallSpawnLocation).
		def("GetPointsScoredThisRound", &TutorialWrapper::GetPointsScoredThisRound).
		def("SetPointsScoredThisRound", &TutorialWrapper::SetPointsScoredThisRound).
		def("GetBallSpawnCount", &TutorialWrapper::GetBallSpawnCount).
		def("SetBallSpawnCount", &TutorialWrapper::SetBallSpawnCount).
		def("GetBallBounceScale", &TutorialWrapper::GetBallBounceScale).
		def("SetBallBounceScale", &TutorialWrapper::SetBallBounceScale).
		def("GetCurrentDebugStepX", &TutorialWrapper::GetCurrentDebugStepX).
		def("SetCurrentDebugStepX", &TutorialWrapper::SetCurrentDebugStepX).
		def("GetCurrentDebugStepY", &TutorialWrapper::GetCurrentDebugStepY).
		def("SetCurrentDebugStepY", &TutorialWrapper::SetCurrentDebugStepY).
		def("GetCurrentDebugStepZ", &TutorialWrapper::GetCurrentDebugStepZ).
		def("SetCurrentDebugStepZ", &TutorialWrapper::SetCurrentDebugStepZ).
		def("GetRedoCount", &TutorialWrapper::GetRedoCount).
		def("SetRedoCount", &TutorialWrapper::SetRedoCount).
		def("GetRedoTotal", &TutorialWrapper::GetRedoTotal).
		def("SetRedoTotal", &TutorialWrapper::SetRedoTotal);


	class_<ReplayDirectorWrapper, bases<ActorWrapper>>("ReplayDirectorWrapper", no_init).
		def("GetSlomoPreScoreTime", &ReplayDirectorWrapper::GetSlomoPreScoreTime).
		def("SetSlomoPreScoreTime", &ReplayDirectorWrapper::SetSlomoPreScoreTime).
		def("GetSlomoPostScoreTime", &ReplayDirectorWrapper::GetSlomoPostScoreTime).
		def("SetSlomoPostScoreTime", &ReplayDirectorWrapper::SetSlomoPostScoreTime).
		def("GetSlomoDefendTime", &ReplayDirectorWrapper::GetSlomoDefendTime).
		def("SetSlomoDefendTime", &ReplayDirectorWrapper::SetSlomoDefendTime).
		def("GetSlomoDefendDistance", &ReplayDirectorWrapper::GetSlomoDefendDistance).
		def("SetSlomoDefendDistance", &ReplayDirectorWrapper::SetSlomoDefendDistance).
		def("GetSlomoTimeDilation", &ReplayDirectorWrapper::GetSlomoTimeDilation).
		def("SetSlomoTimeDilation", &ReplayDirectorWrapper::SetSlomoTimeDilation).
		def("GetMinReplayTime", &ReplayDirectorWrapper::GetMinReplayTime).
		def("SetMinReplayTime", &ReplayDirectorWrapper::SetMinReplayTime).
		def("GetMaxReplayTime", &ReplayDirectorWrapper::GetMaxReplayTime).
		def("SetMaxReplayTime", &ReplayDirectorWrapper::SetMaxReplayTime).
		def("GetReplayPadding", &ReplayDirectorWrapper::GetReplayPadding).
		def("SetReplayPadding", &ReplayDirectorWrapper::SetReplayPadding).
		def("GetHighlightReplayDuration", &ReplayDirectorWrapper::GetHighlightReplayDuration).
		def("SetHighlightReplayDuration", &ReplayDirectorWrapper::SetHighlightReplayDuration).
		def("GetTimeBeforeHighlightReplay", &ReplayDirectorWrapper::GetTimeBeforeHighlightReplay).
		def("SetTimeBeforeHighlightReplay", &ReplayDirectorWrapper::SetTimeBeforeHighlightReplay).
		def("GetReplay", &ReplayDirectorWrapper::GetReplay).
		def("SetReplay", &ReplayDirectorWrapper::SetReplay).
		def("GetFocusCar", &ReplayDirectorWrapper::GetFocusCar).
		def("SetFocusCar", &ReplayDirectorWrapper::SetFocusCar).
		def("GetFocusCarChangeTime", &ReplayDirectorWrapper::GetFocusCarChangeTime).
		def("SetFocusCarChangeTime", &ReplayDirectorWrapper::SetFocusCarChangeTime).
		def("GetFocusBall", &ReplayDirectorWrapper::GetFocusBall).
		def("SetFocusBall", &ReplayDirectorWrapper::SetFocusBall).
		def("GetScoreTime", &ReplayDirectorWrapper::GetScoreTime).
		def("SetScoreTime", &ReplayDirectorWrapper::SetScoreTime).
		def("GetScoreHitIndex", &ReplayDirectorWrapper::GetScoreHitIndex).
		def("SetScoreHitIndex", &ReplayDirectorWrapper::SetScoreHitIndex).
		def("GetScoredGoal", &ReplayDirectorWrapper::GetScoredGoal).
		def("SetScoredGoal", &ReplayDirectorWrapper::SetScoredGoal).
		def("GetbSlomo", &ReplayDirectorWrapper::GetbSlomo).
		def("SetbSlomo", &ReplayDirectorWrapper::SetbSlomo).
		def("GetbSlomoForDefender", &ReplayDirectorWrapper::GetbSlomoForDefender).
		def("SetbSlomoForDefender", &ReplayDirectorWrapper::SetbSlomoForDefender).
		def("GetbAutoSave", &ReplayDirectorWrapper::GetbAutoSave).
		def("SetbAutoSave", &ReplayDirectorWrapper::SetbAutoSave).
		def("GetFocusHitIndex", &ReplayDirectorWrapper::GetFocusHitIndex).
		def("SetFocusHitIndex", &ReplayDirectorWrapper::SetFocusHitIndex).
		def("GetFocusCarIdx", &ReplayDirectorWrapper::GetFocusCarIdx).
		def("SetFocusCarIdx", &ReplayDirectorWrapper::SetFocusCarIdx).
		def("GetReplayStartGameTime", &ReplayDirectorWrapper::GetReplayStartGameTime).
		def("SetReplayStartGameTime", &ReplayDirectorWrapper::SetReplayStartGameTime).
		def("GetBallSpawnTime", &ReplayDirectorWrapper::GetBallSpawnTime).
		def("SetBallSpawnTime", &ReplayDirectorWrapper::SetBallSpawnTime).
		def("GetSoccarGame", &ReplayDirectorWrapper::GetSoccarGame).
		def("SetSoccarGame", &ReplayDirectorWrapper::SetSoccarGame).
		def("GetScoredOnTeam", &ReplayDirectorWrapper::GetScoredOnTeam).
		def("SetScoredOnTeam", &ReplayDirectorWrapper::SetScoredOnTeam).
		def("GetForceCutToFocusActors", &ReplayDirectorWrapper::GetForceCutToFocusActors).
		def("SetForceCutToFocusActors", &ReplayDirectorWrapper::SetForceCutToFocusActors);


	class_<CameraWrapper, bases<ActorWrapper>>("CameraWrapper", no_init).
		def("GetSwivelFastSpeed", &CameraWrapper::GetSwivelFastSpeed).
		def("SetSwivelFastSpeed", &CameraWrapper::SetSwivelFastSpeed).
		def("GetSwivelDieRate", &CameraWrapper::GetSwivelDieRate).
		def("SetSwivelDieRate", &CameraWrapper::SetSwivelDieRate).
		def("GetHorizontalSplitscreenHeightOffset", &CameraWrapper::GetHorizontalSplitscreenHeightOffset).
		def("SetHorizontalSplitscreenHeightOffset", &CameraWrapper::SetHorizontalSplitscreenHeightOffset).
		def("GetHorizontalSplitscreenFOVOffset", &CameraWrapper::GetHorizontalSplitscreenFOVOffset).
		def("SetHorizontalSplitscreenFOVOffset", &CameraWrapper::SetHorizontalSplitscreenFOVOffset).
		def("GetVerticalSplitscreenFOVOffset", &CameraWrapper::GetVerticalSplitscreenFOVOffset).
		def("SetVerticalSplitscreenFOVOffset", &CameraWrapper::SetVerticalSplitscreenFOVOffset).
		def("GetClipRate", &CameraWrapper::GetClipRate).
		def("SetClipRate", &CameraWrapper::SetClipRate).
		def("GetCurrentSwivel", &CameraWrapper::GetCurrentSwivel).
		def("SetCurrentSwivel", &CameraWrapper::SetCurrentSwivel).
		def("GetDemolisher", &CameraWrapper::GetDemolisher).
		def("SetDemolisher", &CameraWrapper::SetDemolisher).
		def("GetbDemolished", &CameraWrapper::GetbDemolished).
		def("SetbDemolished", &CameraWrapper::SetbDemolished);


	class_<ReplaySoccarWrapper, bases<ReplayWrapper>>("ReplaySoccarWrapper", no_init).
		def("GetTeamSize", &ReplaySoccarWrapper::GetTeamSize).
		def("SetTeamSize", &ReplaySoccarWrapper::SetTeamSize).
		def("GetbUnfairBots", &ReplaySoccarWrapper::GetbUnfairBots).
		def("SetbUnfairBots", &ReplaySoccarWrapper::SetbUnfairBots).
		def("GetPrimaryPlayerTeam", &ReplaySoccarWrapper::GetPrimaryPlayerTeam).
		def("SetPrimaryPlayerTeam", &ReplaySoccarWrapper::SetPrimaryPlayerTeam).
		def("GetTeam0Score", &ReplaySoccarWrapper::GetTeam0Score).
		def("SetTeam0Score", &ReplaySoccarWrapper::SetTeam0Score).
		def("GetTeam1Score", &ReplaySoccarWrapper::GetTeam1Score).
		def("SetTeam1Score", &ReplaySoccarWrapper::SetTeam1Score);


	class_<GameEditorWrapper, bases<ServerWrapper>>("GameEditorWrapper", no_init).
		def("GetActiveRoundNumber", &GameEditorWrapper::GetActiveRoundNumber).
		def("SetActiveRoundNumber", &GameEditorWrapper::SetActiveRoundNumber).
		def("GetMaxRounds", &GameEditorWrapper::GetMaxRounds).
		def("SetMaxRounds", &GameEditorWrapper::SetMaxRounds).
		def("GetHistoryPosition", &GameEditorWrapper::GetHistoryPosition).
		def("SetHistoryPosition", &GameEditorWrapper::SetHistoryPosition).
		def("GetMaxUndoHistory", &GameEditorWrapper::GetMaxUndoHistory).
		def("SetMaxUndoHistory", &GameEditorWrapper::SetMaxUndoHistory);


	class_<TrainingEditorWrapper, bases<GameEditorWrapper>>("TrainingEditorWrapper", no_init).
		def("GetMinRoundTime", &TrainingEditorWrapper::GetMinRoundTime).
		def("SetMinRoundTime", &TrainingEditorWrapper::SetMinRoundTime).
		def("GetMaxRoundTime", &TrainingEditorWrapper::GetMaxRoundTime).
		def("SetMaxRoundTime", &TrainingEditorWrapper::SetMaxRoundTime).
		def("GetbNoEditor", &TrainingEditorWrapper::GetbNoEditor).
		def("SetbNoEditor", &TrainingEditorWrapper::SetbNoEditor).
		def("GetbDisplayedRedoPenaltyMessage", &TrainingEditorWrapper::GetbDisplayedRedoPenaltyMessage).
		def("SetbDisplayedRedoPenaltyMessage", &TrainingEditorWrapper::SetbDisplayedRedoPenaltyMessage).
		def("GetbUnsavedChanges", &TrainingEditorWrapper::GetbUnsavedChanges).
		def("SetbUnsavedChanges", &TrainingEditorWrapper::SetbUnsavedChanges).
		def("GetPointsScoredThisRound", &TrainingEditorWrapper::GetPointsScoredThisRound).
		def("SetPointsScoredThisRound", &TrainingEditorWrapper::SetPointsScoredThisRound).
		def("GetShotAttempt", &TrainingEditorWrapper::GetShotAttempt).
		def("SetShotAttempt", &TrainingEditorWrapper::SetShotAttempt).
		def("GetGoalieScore", &TrainingEditorWrapper::GetGoalieScore).
		def("SetGoalieScore", &TrainingEditorWrapper::SetGoalieScore).
		def("GetPlayTestType", &TrainingEditorWrapper::GetPlayTestType).
		def("SetPlayTestType", &TrainingEditorWrapper::SetPlayTestType).
		def("GetGoalMeshBlockers", &TrainingEditorWrapper::GetGoalMeshBlockers).
		def("GetGoalMeshBlockerArchetype", &TrainingEditorWrapper::GetGoalMeshBlockerArchetype).
		def("SetGoalMeshBlockerArchetype", &TrainingEditorWrapper::SetGoalMeshBlockerArchetype).
		def("GetTrainingData", &TrainingEditorWrapper::GetTrainingData).
		def("SetTrainingData", &TrainingEditorWrapper::SetTrainingData).
		def("GetSaveDelayTime", &TrainingEditorWrapper::GetSaveDelayTime).
		def("SetSaveDelayTime", &TrainingEditorWrapper::SetSaveDelayTime).
		def("GetSaveCooldown", &TrainingEditorWrapper::GetSaveCooldown).
		def("SetSaveCooldown", &TrainingEditorWrapper::SetSaveCooldown);


	class_<SaveDataWrapper, bases<ObjectWrapper>>("SaveDataWrapper", no_init).
		def("GetbExactFileMatch", &SaveDataWrapper::GetbExactFileMatch).
		def("SetbExactFileMatch", &SaveDataWrapper::SetbExactFileMatch);


	class_<GameEditorSaveDataWrapper, bases<SaveDataWrapper>>("GameEditorSaveDataWrapper", no_init).
		def("GetTrainingData", &GameEditorSaveDataWrapper::GetTrainingData).
		def("SetTrainingData", &GameEditorSaveDataWrapper::SetTrainingData).
		def("GetPlayerTeamNumber", &GameEditorSaveDataWrapper::GetPlayerTeamNumber).
		def("SetPlayerTeamNumber", &GameEditorSaveDataWrapper::SetPlayerTeamNumber).
		def("GetbUnowned", &GameEditorSaveDataWrapper::GetbUnowned).
		def("SetbUnowned", &GameEditorSaveDataWrapper::SetbUnowned).
		def("GetShotsCompleted", &GameEditorSaveDataWrapper::GetShotsCompleted).
		def("SetShotsCompleted", &GameEditorSaveDataWrapper::SetShotsCompleted);


	class_<TrainingEditorSaveDataWrapper, bases<ObjectWrapper>>("TrainingEditorSaveDataWrapper", no_init).
		def("GetType", &TrainingEditorSaveDataWrapper::GetType).
		def("SetType", &TrainingEditorSaveDataWrapper::SetType).
		def("GetDifficulty", &TrainingEditorSaveDataWrapper::GetDifficulty).
		def("SetDifficulty", &TrainingEditorSaveDataWrapper::SetDifficulty).
		def("GetNumRounds", &TrainingEditorSaveDataWrapper::GetNumRounds).
		def("SetNumRounds", &TrainingEditorSaveDataWrapper::SetNumRounds);


	class_<AttachmentPickup, bases<RumblePickupComponentWrapper>>("AttachmentPickup", no_init);


	class_<BallCarSpringPickup, bases<SpringPickup>>("BallCarSpringPickup", no_init);


	class_<VelcroPickup, bases<RumblePickupComponentWrapper>>("VelcroPickup", no_init).
		def("GetBallOffset", &VelcroPickup::GetBallOffset).
		def("SetBallOffset", &VelcroPickup::SetBallOffset).
		def("GetbUseRealOffset", &VelcroPickup::GetbUseRealOffset).
		def("SetbUseRealOffset", &VelcroPickup::SetbUseRealOffset).
		def("GetbHit", &VelcroPickup::GetbHit).
		def("SetbHit", &VelcroPickup::SetbHit).
		def("GetbBroken", &VelcroPickup::GetbBroken).
		def("SetbBroken", &VelcroPickup::SetbBroken).
		def("GetAfterHitDuration", &VelcroPickup::GetAfterHitDuration).
		def("SetAfterHitDuration", &VelcroPickup::SetAfterHitDuration).
		def("GetPostBreakDuration", &VelcroPickup::GetPostBreakDuration).
		def("SetPostBreakDuration", &VelcroPickup::SetPostBreakDuration).
		def("GetMinBreakForce", &VelcroPickup::GetMinBreakForce).
		def("SetMinBreakForce", &VelcroPickup::SetMinBreakForce).
		def("GetMinBreakTime", &VelcroPickup::GetMinBreakTime).
		def("SetMinBreakTime", &VelcroPickup::SetMinBreakTime).
		def("GetCheckLastTouchRate", &VelcroPickup::GetCheckLastTouchRate).
		def("SetCheckLastTouchRate", &VelcroPickup::SetCheckLastTouchRate).
		def("GetWeldedBall", &VelcroPickup::GetWeldedBall).
		def("SetWeldedBall", &VelcroPickup::SetWeldedBall).
		def("GetOldBallMass", &VelcroPickup::GetOldBallMass).
		def("SetOldBallMass", &VelcroPickup::SetOldBallMass).
		def("GetAttachTime", &VelcroPickup::GetAttachTime).
		def("SetAttachTime", &VelcroPickup::SetAttachTime).
		def("GetLastTouchCheckTime", &VelcroPickup::GetLastTouchCheckTime).
		def("SetLastTouchCheckTime", &VelcroPickup::SetLastTouchCheckTime).
		def("GetBreakTime", &VelcroPickup::GetBreakTime).
		def("SetBreakTime", &VelcroPickup::SetBreakTime);


	class_<BasketballPickup, bases<RumblePickupComponentWrapper>>("BasketballPickup", no_init).
		def("GetBallOffset", &BasketballPickup::GetBallOffset).
		def("SetBallOffset", &BasketballPickup::SetBallOffset).
		def("GetAttachedBallMass", &BasketballPickup::GetAttachedBallMass).
		def("SetAttachedBallMass", &BasketballPickup::SetAttachedBallMass).
		def("GetLaunchForce", &BasketballPickup::GetLaunchForce).
		def("SetLaunchForce", &BasketballPickup::SetLaunchForce).
		def("GetWeldedBall", &BasketballPickup::GetWeldedBall).
		def("SetWeldedBall", &BasketballPickup::SetWeldedBall).
		def("GetOldBallMass", &BasketballPickup::GetOldBallMass).
		def("SetOldBallMass", &BasketballPickup::SetOldBallMass);


	class_<BattarangPickup, bases<BallLassoPickup>>("BattarangPickup", no_init).
		def("GetSpinSpeed", &BattarangPickup::GetSpinSpeed).
		def("SetSpinSpeed", &BattarangPickup::SetSpinSpeed).
		def("GetCurRotation", &BattarangPickup::GetCurRotation).
		def("SetCurRotation", &BattarangPickup::SetCurRotation);


	class_<BoostModPickup, bases<RumblePickupComponentWrapper>>("BoostModPickup", no_init).
		def("GetbUnlimitedBoost", &BoostModPickup::GetbUnlimitedBoost).
		def("SetbUnlimitedBoost", &BoostModPickup::SetbUnlimitedBoost).
		def("GetBoostStrength", &BoostModPickup::GetBoostStrength).
		def("SetBoostStrength", &BoostModPickup::SetBoostStrength).
		def("GetOldBoostStrength", &BoostModPickup::GetOldBoostStrength).
		def("SetOldBoostStrength", &BoostModPickup::SetOldBoostStrength);


	class_<BoostOverridePickup, bases<TargetedPickup>>("BoostOverridePickup", no_init).
		def("GetOtherCar", &BoostOverridePickup::GetOtherCar).
		def("SetOtherCar", &BoostOverridePickup::SetOtherCar);


	class_<CarSpeedPickup, bases<RumblePickupComponentWrapper>>("CarSpeedPickup", no_init).
		def("GetGravityScale", &CarSpeedPickup::GetGravityScale).
		def("SetGravityScale", &CarSpeedPickup::SetGravityScale).
		def("GetAddedForce", &CarSpeedPickup::GetAddedForce).
		def("SetAddedForce", &CarSpeedPickup::SetAddedForce).
		def("GetOrigGravityScale", &CarSpeedPickup::GetOrigGravityScale).
		def("SetOrigGravityScale", &CarSpeedPickup::SetOrigGravityScale);


	class_<DemolishPickup, bases<RumblePickupComponentWrapper>>("DemolishPickup", no_init).
		def("GetDemolishTarget", &DemolishPickup::GetDemolishTarget).
		def("SetDemolishTarget", &DemolishPickup::SetDemolishTarget).
		def("GetDemolishSpeed", &DemolishPickup::GetDemolishSpeed).
		def("SetDemolishSpeed", &DemolishPickup::SetDemolishSpeed).
		def("GetOldTarget", &DemolishPickup::GetOldTarget).
		def("SetOldTarget", &DemolishPickup::SetOldTarget).
		def("GetOldSpeed", &DemolishPickup::GetOldSpeed).
		def("SetOldSpeed", &DemolishPickup::SetOldSpeed);


	class_<HandbrakeOverridePickup, bases<TargetedPickup>>("HandbrakeOverridePickup", no_init).
		def("GetOtherCar", &HandbrakeOverridePickup::GetOtherCar).
		def("SetOtherCar", &HandbrakeOverridePickup::SetOtherCar);


	class_<HitForcePickup, bases<RumblePickupComponentWrapper>>("HitForcePickup", no_init).
		def("GetbBallForce", &HitForcePickup::GetbBallForce).
		def("SetbBallForce", &HitForcePickup::SetbBallForce).
		def("GetbCarForce", &HitForcePickup::GetbCarForce).
		def("SetbCarForce", &HitForcePickup::SetbCarForce).
		def("GetbDemolishCars", &HitForcePickup::GetbDemolishCars).
		def("SetbDemolishCars", &HitForcePickup::SetbDemolishCars).
		def("GetBallHitForce", &HitForcePickup::GetBallHitForce).
		def("SetBallHitForce", &HitForcePickup::SetBallHitForce).
		def("GetCarHitForce", &HitForcePickup::GetCarHitForce).
		def("SetCarHitForce", &HitForcePickup::SetCarHitForce).
		def("GetMinFXTime", &HitForcePickup::GetMinFXTime).
		def("SetMinFXTime", &HitForcePickup::SetMinFXTime).
		def("GetOrigBallHitForce", &HitForcePickup::GetOrigBallHitForce).
		def("SetOrigBallHitForce", &HitForcePickup::SetOrigBallHitForce).
		def("GetOrigCarHitForce", &HitForcePickup::GetOrigCarHitForce).
		def("SetOrigCarHitForce", &HitForcePickup::SetOrigCarHitForce).
		def("GetLastFXTime", &HitForcePickup::GetLastFXTime).
		def("SetLastFXTime", &HitForcePickup::SetLastFXTime);


	class_<SwapperPickup, bases<TargetedPickup>>("SwapperPickup", no_init).
		def("GetOtherCar", &SwapperPickup::GetOtherCar).
		def("SetOtherCar", &SwapperPickup::SetOtherCar);


	class_<TimeBombPickup, bases<RumblePickupComponentWrapper>>("TimeBombPickup", no_init).
		def("GetRadius", &TimeBombPickup::GetRadius).
		def("SetRadius", &TimeBombPickup::SetRadius).
		def("GetAlmostReadyDuration", &TimeBombPickup::GetAlmostReadyDuration).
		def("SetAlmostReadyDuration", &TimeBombPickup::SetAlmostReadyDuration).
		def("GetStartMatSpeed", &TimeBombPickup::GetStartMatSpeed).
		def("SetStartMatSpeed", &TimeBombPickup::SetStartMatSpeed).
		def("GetAlmostReadyMatSpeed", &TimeBombPickup::GetAlmostReadyMatSpeed).
		def("SetAlmostReadyMatSpeed", &TimeBombPickup::SetAlmostReadyMatSpeed).
		def("GetImpulseForce", &TimeBombPickup::GetImpulseForce).
		def("SetImpulseForce", &TimeBombPickup::SetImpulseForce).
		def("GetCarVerticalForce", &TimeBombPickup::GetCarVerticalForce).
		def("SetCarVerticalForce", &TimeBombPickup::SetCarVerticalForce).
		def("GetCarTorque", &TimeBombPickup::GetCarTorque).
		def("SetCarTorque", &TimeBombPickup::SetCarTorque).
		def("GetbDemolish", &TimeBombPickup::GetbDemolish).
		def("SetbDemolish", &TimeBombPickup::SetbDemolish).
		def("GetbImpulse", &TimeBombPickup::GetbImpulse).
		def("SetbImpulse", &TimeBombPickup::SetbImpulse);


	PYTHON_ARRAY(ActorWrapper)
		PYTHON_ARRAY(CarWrapper)
		PYTHON_ARRAY(BallWrapper)
		PYTHON_ARRAY(PriWrapper)
		PYTHON_ARRAY(WheelWrapper)
		PYTHON_ARRAY(RBActorWrapper)
		PYTHON_ARRAY(CarComponentWrapper)
		PYTHON_ARRAY(TeamWrapper)
		PYTHON_ARRAY(GoalWrapper)
}

