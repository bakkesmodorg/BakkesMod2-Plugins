#pragma once
#define BOOST_PYTHON_STATIC_LIB  
#include "boost/python/detail/wrap_python.hpp"
#include <boost/python.hpp>
#include "bakkesmod/wrappers/ballwrapper.h"
#include "bakkesmod/wrappers/carcomponent/boostwrapper.h"
#include "bakkesmod/wrappers/camerawrapper.h"
#include "bakkesmod/wrappers/carwrapper.h"
#include "bakkesmod/wrappers/gameeventwrapper.h"
#include "bakkesmod/wrappers/priwrapper.h"
#include "bakkesmod/wrappers/serverwrapper.h"
#include "bakkesmod/wrappers/replaywrapper.h"
#include "bakkesmod/wrappers/tutorialwrapper.h"

using namespace boost::python;

//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(log_overloads, ConsoleWrapper::log, 1, 2);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(set_angular_overloads, ActorWrapper::SetAngularVelocity, 1, 2);
void (CVarWrapper::*setString)(string) = &CVarWrapper::setValue;
void (CVarWrapper::*setInt)(int) = &CVarWrapper::setValue;
void (CVarWrapper::*setFloat)(float) = &CVarWrapper::setValue;

//class Helpers {};

#define PYTHON_ARRAY(WrappedArrayClass) \
	class_<ArrayWrapper<WrappedArrayClass>>("ArrayWrapper#WrappedArrayClass", no_init).\
		def("count", &ArrayWrapper<CarWrapper>::Count).\
		def("get", &ArrayWrapper<CarWrapper>::Get);

BOOST_PYTHON_MODULE(bakkesmod)
{
	class_<BakPy>("BakPy", no_init).
		def("set_timeout", &BakPy::set_timeout).
		def("vector_to_rotator", VectorToRotator).staticmethod("vector_to_rotator").
		def("rotator_to_vector", RotatorToVector).staticmethod("rotator_to_vector");;//.staticmethod("set_timeout");

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
		def("get_cvar_name", &CVarWrapper::getCVarName).
		def("get_int_value", &CVarWrapper::getIntValue).
		def("get_float_value", &CVarWrapper::getFloatValue).
		def("get_bool_value", &CVarWrapper::getBoolValue).
		def("get_string_value", &CVarWrapper::getStringValue).
		def("get_description", &CVarWrapper::getDescription).
		def("notify", &CVarWrapper::notify).
		def("set_value", setString).
		def("set_value", setInt).
		def("set_value", setFloat);

	class_<CVarManagerWrapper>("CVarManagerWrapper", no_init).
		def("execute_command", &CVarManagerWrapper::executeCommand).
		//.def("register_notifier", CVarManagerWrapper::registerNotifier). //No need to give acces to this as t heyre C++ callbacks and python scripts should have standalone functions
		def("register_cvar", &CVarManagerWrapper::registerCvar).
		def("log", &CVarManagerWrapper::log).
		def("get_cvar", &CVarManagerWrapper::getCvar);

	class_<GameWrapper, boost::noncopyable>("GameWrapper", no_init).
		def("is_in_tutorial", &GameWrapper::IsInTutorial).
		def("is_in_replay", &GameWrapper::IsInReplay).
		def("is_in_custom_training", &GameWrapper::IsInCustomTraining).
		def("get_game_event", &GameWrapper::GetGameEvent).
		def("get_game_event_as_tutorial", &GameWrapper::GetGameEventAsTutorial).
		def("get_game_event_as_server", &GameWrapper::GetGameEventAsServer).
		def("get_game_event_as_replay", &GameWrapper::GetGameEventAsReplay).
		def("get_local_car", &GameWrapper::GetLocalCar).
		def("get_camera", &GameWrapper::GetCamera).
		//def("set_timeout", &GameWrapper::SetTimeout). //time in MS
		def("execute", &GameWrapper::Execute). //Use this when calling from a different thread
		def("register_drawable", &GameWrapper::RegisterDrawable).
		def("get_fname_by_index", &GameWrapper::GetFNameByIndex);
		//def("get_solid_hook", &GameWrapper::GetSolidHook).
		//def("get_calculator", &GameWrapper::GetCalculator);

	class_<PredictionInfo>("PredictionInfo").
		def(init<>()).
		def_readwrite("time", &PredictionInfo::Time).
		def_readwrite("location", &PredictionInfo::Location).
		def_readwrite("velocity", &PredictionInfo::Velocity).
		def_readwrite("hit_wall", &PredictionInfo::HitWall).
		def_readwrite("hit_floor", &PredictionInfo::HitFloor);

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
		def_readwrite("x", &Vector::X).
		def_readwrite("y", &Vector::Y).
		def_readwrite("z", &Vector::Z).
		def(self + Vector()).
		def(self - Vector()).
		def(self * Vector()).
		def(self / Vector()).
		def("magnitude", &Vector::magnitude).
		def("normalize", &Vector::normalize).
		def("clone", &Vector::clone).
		def("dot", &Vector::dot).
		def("cross", &Vector::cross).
		def("lerp", &Vector::lerp).
		def("slerp", &Vector::slerp);


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

	class_<ActorWrapper>("ActorWrapper", no_init).
		def("get_velocity", &ActorWrapper::GetVelocity).
		def("set_velocity", &ActorWrapper::SetVelocity).
		def("get_location", &ActorWrapper::GetLocation).
		def("set_location", &ActorWrapper::SetLocation).
		def("get_velocity", &ActorWrapper::GetVelocity).
		def("set_velocity", &ActorWrapper::SetVelocity).
		def("add_velocity", &ActorWrapper::AddVelocity).
		def("get_rotation", &ActorWrapper::GetRotation).
		def("set_rotation", &ActorWrapper::SetRotation).
		def("set_torque", &ActorWrapper::SetTorque).
		def("stop", &ActorWrapper::Stop).
		//def("freeze", &ActorWrapper::Freeze).
		//def("is_hooked", &ActorWrapper::IsHooked).
		def("get_angular_velocity", &ActorWrapper::GetAngularVelocity).
		def("set_angular_velocity", &ActorWrapper::SetAngularVelocity, set_angular_overloads()).
		def("is_null", &ActorWrapper::IsNull);

	class_<BallWrapper, bases<ActorWrapper>>("BallWrapper", no_init).
		def("get_gravity_scale", &BallWrapper::GetGravityScale).
		def("set_gravity_scale", &BallWrapper::SetGravityScale).
		def("get_explosion_time", &BallWrapper::GetExplosionTime).
		def("set_explosion_time", &BallWrapper::SetExplosionTime).
		def("get_last_touch_time", &BallWrapper::GetLastTouchTime);

	class_<BoostWrapper, bases<ActorWrapper>>("BoostWrapper", no_init).
		def("get_max_boost_amount", &BoostWrapper::GetMaxBoostAmount).
		def("set_max_boost_amount", &BoostWrapper::SetMaxBoostAmount).
		def("get_start_boost_amount", &BoostWrapper::GetStartBoostAmount).
		def("set_start_boost_amount", &BoostWrapper::SetStartBoostAmount).
		def("get_is_active", &BoostWrapper::GetIsActive).
		def("set_is_active", &BoostWrapper::SetIsActive).
		def("get_boost_consumption_rate", &BoostWrapper::GetBoostConsumptionRate).
		def("set_boost_consumption_rate", &BoostWrapper::SetBoostConsumptionRate).
		def("get_boost_force", &BoostWrapper::GetBoostForce).
		def("set_boost_force", &BoostWrapper::SetBoostForce).
		def("get_min_boost_time", &BoostWrapper::GetMinBoostTime).
		def("set_min_boost_time", &BoostWrapper::SetMinBoostTime).
		def("get_recharge_rate", &BoostWrapper::GetRechargeRate).
		def("set_recharge_rate", &BoostWrapper::SetRechargeRate).
		def("get_recharge_delay", &BoostWrapper::GetRechargeDelay).
		def("set_recharge_delay", &BoostWrapper::SetRechargeDelay).
		//def("get_only_apply_recharge_to_my_half", &BoostWrapper::GetOnlyApplyRechargeToMyHalf).
		//def("set_only_apply_recharge_to_my_half", &BoostWrapper::SetOnlyApplyRechargeToMyHalf).
		//def("get_only_apply_recharge_to_opposing_half", &BoostWrapper::GetOnlyApplyRechargeToOpposingHalf).
		//def("set_only_apply_recharge_to_opposing_half", &BoostWrapper::SetOnlyApplyRechargeToOpposingHalf).
		//def("get_demolish_on_empty_my_half", &BoostWrapper::GetDemolishOnEmptyMyHalf).
		//def("set_demolish_on_empty_my_half", &BoostWrapper::SetDemolishOnEmptyMyHalf).
		//def("get_demolish_on_empty_opposing_half", &BoostWrapper::GetDemolishOnEmptyOpposingHalf).
		//def("set_demolish_on_empty_opposing_half", &BoostWrapper::SetDemolishOnEmptyOpposingHalf).
		//def("get_cached_on_my_half", &BoostWrapper::GetCachedOnMyHalf).
		//def("set_cached_on_my_half", &BoostWrapper::SetCachedOnMyHalf).
		def("get_current_boost_amount", &BoostWrapper::GetCurrentBoostAmount).
		def("get_is_unlimited_boost", &BoostWrapper::GetIsUnlimitedBoost).
		def("set_boost_amount", &BoostWrapper::SetBoostAmount).
		def("add_boost_amount", &BoostWrapper::AddBoostAmount).
		def("set_is_unlimited_boost", &BoostWrapper::SetIsUnlimitedBoost);

	//class_<CalculationHelpers>("CalculationHelpers", no_init).
	//	def("predict_ball", &CalculationHelpers::PredictBall);

	class_<CameraWrapper>("CameraWrapper", no_init).
		def("get_location", &CameraWrapper::GetLocation).
		def("set_location", &CameraWrapper::SetLocation).
		def("get_rotation", &CameraWrapper::GetRotation).
		def("set_rotation", &CameraWrapper::SetRotation).
		def("get_camerasettings", &CameraWrapper::GetCameraSettings).
		def("_is_camera_shake_on", &CameraWrapper::IsCameraShakeOn).
		def("get_POV", &CameraWrapper::GetPOV).
		def("set_POV", &CameraWrapper::SetPOV).
		def("set_FOV", &CameraWrapper::SetFOV).
		def("get_FOV", &CameraWrapper::GetFOV).
		def("set_locked_FOV", &CameraWrapper::SetLockedFOV).
		def("get_camera_as_actor", &CameraWrapper::GetCameraAsActor).
		def("get_camera_state", &CameraWrapper::GetCameraState).
		def("linterp", &CameraWrapper::linterp);

	class_<CanvasWrapper>("CanvasWrapper", no_init).
		def("set_position", &CanvasWrapper::SetPosition).
		def("get_position", &CanvasWrapper::GetPosition).
		def("set_color", &CanvasWrapper::SetColor).//0-255
		def("draw_box", &CanvasWrapper::DrawBox).
		def("fill_box", &CanvasWrapper::FillBox).
		def("draw_string", &CanvasWrapper::DrawString).
		def("draw_line", &CanvasWrapper::DrawLine).
		def("draw_rect", &CanvasWrapper::DrawRect).
		def("project", &CanvasWrapper::Project);

	class_<CarWrapper, bases<ActorWrapper>>("CarWrapper", no_init).
		def("get_is_driving", &CarWrapper::GetIsDriving).
		def("set_is_driving", &CarWrapper::SetIsDriving).
		def("get_is_on_ground", &CarWrapper::GetIsOnGround).
		def("set_is_on_ground", &CarWrapper::SetIsOnGround).
		def("get_has_jumped", &CarWrapper::GetHasJumped).
		def("set_has_jumped", &CarWrapper::SetHasJumped).
		def("get_has_double_jumped", &CarWrapper::GetHasDoubleJumped).
		def("set_has_double_jumped", &CarWrapper::SetHasDoubleJumped).
		def("get_added_ball_force_multiplier", &CarWrapper::GetAddedBallForceMultiplier).
		def("set_added_ball_force_multiplier", &CarWrapper::SetAddedBallForceMultiplier).
		def("get_disable_gravity", &CarWrapper::GetDisableGravity).
		def("set_disable_gravity", &CarWrapper::SetDisableGravity).
		def("get_demolish_on_opposing_ground", &CarWrapper::GetDemolishOnOpposingGround).
		def("set_demolish_on_opposing_ground", &CarWrapper::SetDemolishOnOpposingGround).
		def("get_was_on_opposing_ground", &CarWrapper::GetWasOnOpposingGround).
		def("set_was_on_opposing_ground", &CarWrapper::SetWasOnOpposingGround).
		def("get_demolish_on_goal_zone", &CarWrapper::GetDemolishOnGoalZone).
		def("set_demolish_on_goal_zone", &CarWrapper::SetDemolishOnGoalZone).
		def("get_was_in_goal_zone", &CarWrapper::GetWasInGoalZone).
		def("set_was_in_goal_zone", &CarWrapper::SetWasInGoalZone).
		def("get_override_handbrake_on", &CarWrapper::GetOverrideHandbrakeOn).
		def("set_override_handbrake_on", &CarWrapper::SetOverrideHandbrakeOn).
		def("get_override_boost_on", &CarWrapper::GetOverrideBoostOn).
		def("set_override_boost_on", &CarWrapper::SetOverrideBoostOn).
		def("get_added_car_force_multiplier", &CarWrapper::GetAddedCarForceMultiplier).
		def("set_added_car_force_multiplier", &CarWrapper::SetAddedCarForceMultiplier).
		def("get_max_time_for_dodge", &CarWrapper::GetMaxTimeForDodge).
		def("set_max_time_for_dodge", &CarWrapper::SetMaxTimeForDodge).
		def("get_last_wheels_hit_ball_time", &CarWrapper::GetLastWheelsHitBallTime).
		def("set_last_wheels_hit_ball_time", &CarWrapper::SetLastWheelsHitBallTime).
		def("get_boost", &CarWrapper::GetBoost).
		def("get_pri", &CarWrapper::GetPRI).
		def("is_boost_cheap", &CarWrapper::IsBoostCheap).
		def("set_boost_cheap", &CarWrapper::SetBoostCheap).
		def("set_car_rotation", &CarWrapper::SetCarRotation).
		def("force_boost", &CarWrapper::ForceBoost).
		def("get_owner_name", &CarWrapper::GetOwnerName).
		def("unfreeze", &CarWrapper::Unfreeze).
		def("get_input", &CarWrapper::GetInput).
		def("set_input", &CarWrapper::SetInput);

	class_<GameEventWrapper, bases<ActorWrapper>>("GameEventWrapper", no_init).
		def("get_ball", &GameEventWrapper::GetBall).
		def("is_training", &GameEventWrapper::IsTraining).
		def("is_in_free_play", &GameEventWrapper::IsInFreePlay).
		def("is_server", &GameEventWrapper::IsServer).
		def("kick_bots", &GameEventWrapper::KickBots).
		def("get_pri_car", &GameEventWrapper::GetPRICar);

	class_<PriWrapper, bases<ActorWrapper>>("PriWrapper", no_init).
		def("get_score", &PriWrapper::GetScore).
		def("set_score", &PriWrapper::SetScore).
		def("get_goals", &PriWrapper::GetGoals).
		def("set_goals", &PriWrapper::SetGoals).
		def("get_assists", &PriWrapper::GetAssists).
		def("set_assists", &PriWrapper::SetAssists).
		def("get_saves", &PriWrapper::GetSaves).
		def("set_saves", &PriWrapper::SetSaves).
		def("get_shots", &PriWrapper::GetShots).
		def("set_shots", &PriWrapper::SetShots).
		def("get_demolishes", &PriWrapper::GetDemolishes).
		def("set_demolishes", &PriWrapper::SetDemolishes).
		def("get_using_secondary_camera", &PriWrapper::GetUsingSecondaryCamera).
		def("set_using_secondary_camera", &PriWrapper::SetUsingSecondaryCamera).
		def("get_using_free_camera", &PriWrapper::GetUsingFreeCamera).
		def("set_using_free_camera", &PriWrapper::SetUsingFreeCamera).
		def("get_ball_touch_count", &PriWrapper::GetBallTouchCount).
		def("set_ball_touch_count", &PriWrapper::SetBallTouchCount).
		def("get_car_touch_count", &PriWrapper::GetCarTouchCount).
		def("set_car_touch_count", &PriWrapper::SetCarTouchCount).
		def("get_respawn_time_remaining", &PriWrapper::GetRespawnTimeRemaining).
		def("set_respawn_time_remaining", &PriWrapper::SetRespawnTimeRemaining).
		def("get_ready", &PriWrapper::GetReady).
		def("set_ready", &PriWrapper::SetReady).
		def("get_exact_ping", &PriWrapper::GetExactPing).
		def("set_exact_ping", &PriWrapper::SetExactPing).
		def("get_is_bot", &PriWrapper::GetIsBot).
		def("set_is_bot", &PriWrapper::SetIsBot).
		def("get_player_id", &PriWrapper::GetPlayerID).
		def("set_player_id", &PriWrapper::SetPlayerID).
		def("get_car", &PriWrapper::GetCar).
		def("get_team_index", &PriWrapper::GetTeamIndex).
		def("get_player_name", &PriWrapper::GetPlayerName).
		def("is_spectator", &PriWrapper::IsSpectator).
		def("get_body", &PriWrapper::GetBody);

	class_<ServerWrapper, bases<GameEventWrapper>>("ServerWrapper", no_init).
		def("get_time_remaining", &ServerWrapper::GetTimeRemaining).
		def("set_time_remaining", &ServerWrapper::SetTimeRemaining).
		def("get_podium_time", &ServerWrapper::GetPodiumTime).
		def("set_podium_time", &ServerWrapper::SetPodiumTime).
		def("get_game_speed", &ServerWrapper::GetGameSpeed).
		def("set_game_speed", &ServerWrapper::SetGameSpeed).
		def("get_seconds_elapsed", &ServerWrapper::GetSecondsElapsed).
		def("set_seconds_elapsed", &ServerWrapper::SetSecondsElapsed).
		def("get_game_time", &ServerWrapper::GetGameTime).
		def("set_game_time", &ServerWrapper::SetGameTime).
		def("get_max_score", &ServerWrapper::GetMaxScore).
		def("set_max_score", &ServerWrapper::SetMaxScore).
		def("get_total_game_time_played", &ServerWrapper::GetTotalGameTimePlayed).
		def("set_total_game_time_played", &ServerWrapper::SetTotalGameTimePlayed).
		//def("get_games_played", &ServerWrapper::GetGamesPlayed).
		//def("set_games_played", &ServerWrapper::SetGamesPlayed).
		def("get_play_replays", &ServerWrapper::GetPlayReplays).
		def("set_play_replays", &ServerWrapper::SetPlayReplays).
		def("get_ball_has_been_hit", &ServerWrapper::GetBallHasBeenHit).
		def("set_ball_has_been_hit", &ServerWrapper::SetBallHasBeenHit).
		def("get_over_time", &ServerWrapper::GetOverTime).
		def("set_over_time", &ServerWrapper::SetOverTime).
		def("get_unlimited_time", &ServerWrapper::GetUnlimitedTime).
		def("set_unlimited_time", &ServerWrapper::SetUnlimitedTime).
		def("get_disable_goal_delay", &ServerWrapper::GetDisableGoalDelay).
		def("set_disable_goal_delay", &ServerWrapper::SetDisableGoalDelay).
		def("get_match_ended", &ServerWrapper::GetMatchEnded).
		def("set_match_ended", &ServerWrapper::SetMatchEnded).
		def("get_total_game_balls", &ServerWrapper::GetTotalGameBalls).
		def("set_total_game_balls", &ServerWrapper::SetTotalGameBalls).
		def("get_post_goal_time", &ServerWrapper::GetPostGoalTime).
		def("set_post_goal_time", &ServerWrapper::SetPostGoalTime).
		def("get_seconds_remaining_countdown", &ServerWrapper::GetSecondsRemainingCountdown).
		def("set_seconds_remaining_countdown", &ServerWrapper::SetSecondsRemainingCountdown).
		def("get_assist_max_time", &ServerWrapper::GetAssistMaxTime).
		def("set_assist_max_time", &ServerWrapper::SetAssistMaxTime).
		def("get_ball_has_been_hit_start_delay", &ServerWrapper::GetBallHasBeenHitStartDelay).
		def("set_ball_has_been_hit_start_delay", &ServerWrapper::SetBallHasBeenHitStartDelay).
		def("get_lobby_time", &ServerWrapper::GetLobbyTime).
		def("set_lobby_time", &ServerWrapper::SetLobbyTime).
		def("get_lobby_countdown", &ServerWrapper::GetLobbyCountdown).
		def("set_lobby_countdown", &ServerWrapper::SetLobbyCountdown).
		def("get_lobby_spawn_restart_time", &ServerWrapper::GetLobbySpawnRestartTime).
		def("set_lobby_spawn_restart_time", &ServerWrapper::SetLobbySpawnRestartTime).
		def("get_countdown_time", &ServerWrapper::GetCountdownTime).
		def("set_countdown_time", &ServerWrapper::SetCountdownTime).
		def("get_team", &ServerWrapper::GetTeam).
		def("get_pris", &ServerWrapper::GetPRIs).
		def("get_players", &ServerWrapper::GetPlayers).
		def("get_goal_location", &ServerWrapper::GetGoalLocation).
		def("get_scored_on_team", &ServerWrapper::GetScoredOnTeam).
		def("spawn_ball", &ServerWrapper::SpawnBall).
		def("get_balls", &ServerWrapper::GetBalls).
		def("remove_ball", &ServerWrapper::RemoveBall).
		def("move_to_ground", &ServerWrapper::MoveToGround).
		//def("spawn_bot_with_ai", &ServerWrapper::SpawnBotWithAI).
		//def("get_user_input", &ServerWrapper::GetUserInput).
		def("start_countdown", &ServerWrapper::StartCountdown).
		def("reset_balls", &ServerWrapper::ResetBalls).
		def("set_game_state_time", &ServerWrapper::SetGameStateTime).
		def("set_all_driving", &ServerWrapper::SetAllDriving);

	class_<ReplayWrapper, bases<ServerWrapper>>("ReplayWrapper", no_init).
		def("get_view_target", &ReplayWrapper::GetViewTarget).
		def("get_replay_time_elapsed", &ReplayWrapper::GetReplayTimeElapsed).
		def("get_current_replay_frame", &ReplayWrapper::GetCurrentReplayFrame).
		def("add_key_frame", &ReplayWrapper::AddKeyFrame).
		def("remove_key_frame", &ReplayWrapper::RemoveKeyFrame).
		def("skip_to_time", &ReplayWrapper::SkipToTime);


	class_<TeamWrapper, bases<ActorWrapper>>("TeamWrapper", no_init).
		def("get_score", &TeamWrapper::GetScore).
		def("set_score", &TeamWrapper::SetScore).
		def("get_team_size", &TeamWrapper::GetTeamSize).
		def("get_index", &TeamWrapper::GetIndex).
		def("get_bot_count", &TeamWrapper::GetBotCount).
		def("get_human_count", &TeamWrapper::GetHumanCount);

	class_<TutorialWrapper, bases<ServerWrapper>>("TutorialWrapper", no_init).
		def("get_team_num", &TutorialWrapper::GetTeamNum).
		def("set_team_num", &TutorialWrapper::SetTeamNum).
		def("get_redo_count", &TutorialWrapper::GetRedoCount).
		def("set_redo_count", &TutorialWrapper::SetRedoCount).
		def("get_redo_total", &TutorialWrapper::GetRedoTotal).
		def("set_redo_total", &TutorialWrapper::SetRedoTotal).
		def("get_countdown_time_left", &TutorialWrapper::GetCountdownTimeLeft).
		def("set_countdown_time_left", &TutorialWrapper::SetCountdownTimeLeft).
		def("get_ball_goal_num", &TutorialWrapper::GetBallGoalNum).
		def("set_ball_goal_num", &TutorialWrapper::SetBallGoalNum).
		def("get_only_score_in_ball_goal_num", &TutorialWrapper::GetOnlyScoreInBallGoalNum).
		def("set_only_score_in_ball_goal_num", &TutorialWrapper::SetOnlyScoreInBallGoalNum).
		def("get_is_unlimited_boost", &TutorialWrapper::GetIsUnlimitedBoost).
		def("set_is_unlimited_boost", &TutorialWrapper::SetIsUnlimitedBoost).
		def("get_show_boost_meter", &TutorialWrapper::GetShowBoostMeter).
		def("set_show_boost_meter", &TutorialWrapper::SetShowBoostMeter).
		def("get_ball_bounce_scale", &TutorialWrapper::GetBallBounceScale).
		def("set_ball_bounce_scale", &TutorialWrapper::SetBallBounceScale).
		def("get_ball_spawn_location", &TutorialWrapper::GetBallSpawnLocation).
		def("set_ball_spawn_location", &TutorialWrapper::SetBallSpawnLocation).
		def("get_ball_start_velocity", &TutorialWrapper::GetBallStartVelocity).
		def("set_ball_start_velocity", &TutorialWrapper::SetBallStartVelocity).
		def("get_total_field_extent", &TutorialWrapper::GetTotalFieldExtent).
		def("set_total_field_extent", &TutorialWrapper::SetTotalFieldExtent).
		def("redo", &TutorialWrapper::Redo).
		def("get_car_spawn_location", &TutorialWrapper::GetCarSpawnLocation).
		def("set_car_spawn_location", &TutorialWrapper::SetCarSpawnLocation).
		def("get_car_spawn_rotation", &TutorialWrapper::GetCarSpawnRotation).
		def("set_car_spawn_rotation", &TutorialWrapper::SetCarSpawnRotation).
		def("get_game_car", &TutorialWrapper::GetGameCar).
		def("is_ball_moving_towards_goal", &TutorialWrapper::IsBallMovingTowardsGoal).
		def("is_in_goal", &TutorialWrapper::IsInGoal).
		def("disable_goal_reset", &TutorialWrapper::DisableGoalReset).
		def("enable_goal_reset", &TutorialWrapper::EnableGoalReset).
		//def("spawn_car", &TutorialWrapper::SpawnCar).
		def("generate_shot", &TutorialWrapper::GenerateShot).
		def("generate_goal_aim_location", &TutorialWrapper::GenerateGoalAimLocation).
		def("get_goal_extent", &TutorialWrapper::GetGoalExtent);

	PYTHON_ARRAY(CarWrapper)
	PYTHON_ARRAY(PriWrapper)
	PYTHON_ARRAY(BallWrapper)
}

