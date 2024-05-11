#pragma once
#include "../includes.hpp"
#include "il2cpp.hpp"
#include <map>
#include <string>
#include <variant>

#define print(a, args) printf("%s(%s:%d) " a,  __func__,__FILE__, __LINE__, ##args)
#define println(a, args) print(a "\n", ##args)
#define min(a,b)            (((a) < (b)) ? (a) : (b))

bool is_valid(uintptr_t variable);

template<typename T, typename... Args>
inline T call(const char* func, Args... args);

static auto getMainCamera = reinterpret_cast<uintptr_t(*)()>(il2cpp::methods::resolve_icall(xorstr_("UnityEngine.Camera::get_main()")));

struct raidType {
	std::string type;
	float count;
};

struct raidItem {
	std::clock_t time;
	std::string raid_id;
	std::string grid;
	Vector3 pos;
	std::vector<raidType> explosions;
};

struct weapon_data {
	float initial_velocity;
	float gravity_modifier;
	float drag;
	float initial_distance;
};

struct authList {
	std::string name;
	long userid;
	std::string steam64;
};

struct playerlist_player {
	std::string name;
	std::string steamid;
};

struct playerlist_data {
	std::string square;
	float x;
	float y;
	float z;
	float last_check;
	float fail_check;
	std::vector<playerlist_player> players;
};


typedef struct Str
{
	char stub[0x10];
	int len;
	wchar_t str[1];
} *str;

enum UIBlackoutOverlays : int
{
	FULLBLACK = 0,
	BINOCULAR = 1,
	SCOPE = 2,
	HELMETSLIT = 3,
	SNORKELGOGGLE = 4,
	NVG = 5,
	FULLWHITE = 6,
	SUNGLASSES = 7,
	NONE = 64
};

enum BoneList : int
{
	l_hip = 1,
	l_knee,
	l_foot,
	l_toe,
	l_ankle_scale,
	pelvis,
	penis,
	GenitalCensor,
	GenitalCensor_LOD0,
	Inner_LOD0,
	GenitalCensor_LOD1,
	GenitalCensor_LOD2,
	r_hip,
	r_knee,
	r_foot,
	r_toe,
	r_ankle_scale,
	spine1,
	spine1_scale,
	spine2,
	spine3,
	spine4,
	l_clavicle,
	l_upperarm,
	l_forearm,
	l_hand,
	l_index1,
	l_index2,
	l_index3,
	l_little1,
	l_little2,
	l_little3,
	l_middle1,
	l_middle2,
	l_middle3,
	l_prop,
	l_ring1,
	l_ring2,
	l_ring3,
	l_thumb1,
	l_thumb2,
	l_thumb3,
	IKtarget_righthand_min,
	IKtarget_righthand_max,
	l_ulna,
	neck,
	head,
	jaw,
	eyeTranform,
	l_eye,
	l_Eyelid,
	r_eye,
	r_Eyelid,
	r_clavicle,
	r_upperarm,
	r_forearm,
	r_hand,
	r_index1,
	r_index2,
	r_index3,
	r_little1,
	r_little2,
	r_little3,
	r_middle1,
	r_middle2,
	r_middle3,
	r_prop,
	r_ring1,
	r_ring2,
	r_ring3,
	r_thumb1,
	r_thumb2,
	r_thumb3,
	IKtarget_lefthand_min,
	IKtarget_lefthand_max,
	r_ulna,
	l_breast,
	r_breast,
	BoobCensor,
	BreastCensor_LOD0,
	BreastCensor_LOD1,
	BreastCensor_LOD2,
	collision,
	displacement
};

enum class KeyCode
{
	Backspace = 8,
	Delete = 127,
	Tab = 9,
	Clear = 12,
	Return = 13,
	Pause = 19,
	Escape = 27,
	Space = 32,
	Keypad0 = 256,
	Keypad1 = 257,
	Keypad2 = 258,
	Keypad3 = 259,
	Keypad4 = 260,
	Keypad5 = 261,
	Keypad6 = 262,
	Keypad7 = 263,
	Keypad8 = 264,
	Keypad9 = 265,
	KeypadPeriod = 266,
	KeypadDivide = 267,
	KeypadMultiply = 268,
	KeypadMinus = 269,
	KeypadPlus = 270,
	KeypadEnter = 271,
	KeypadEquals = 272,
	UpArrow = 273,
	DownArrow = 274,
	RightArrow = 275,
	LeftArrow = 276,
	Insert = 277,
	Home = 278,
	End = 279,
	PageUp = 280,
	PageDown = 281,
	F1 = 282,
	F2 = 283,
	F3 = 284,
	F4 = 285,
	F5 = 286,
	F6 = 287,
	F7 = 288,
	F8 = 289,
	F9 = 290,
	F10 = 291,
	F11 = 292,
	F12 = 293,
	F13 = 294,
	F14 = 295,
	F15 = 296,
	Alpha0 = 48,
	Alpha1 = 49,
	Alpha2 = 50,
	Alpha3 = 51,
	Alpha4 = 52,
	Alpha5 = 53,
	Alpha6 = 54,
	Alpha7 = 55,
	Alpha8 = 56,
	Alpha9 = 57,
	Exclaim = 33,
	DoubleQuote = 34,
	Hash = 35,
	Dollar = 36,
	Percent = 37,
	Ampersand = 38,
	Quote = 39,
	LeftParen = 40,
	RightParen = 41,
	Asterisk = 42,
	Plus = 43,
	Comma = 44,
	Minus = 45,
	Period = 46,
	Slash = 47,
	Colon = 58,
	Semicolon = 59,
	Less = 60,
	Equals = 61,
	Greater = 62,
	Question = 63,
	At = 64,
	LeftBracket = 91,
	Backslash = 92,
	RightBracket = 93,
	Caret = 94,
	Underscore = 95,
	BackQuote = 96,
	A = 97,
	B = 98,
	C = 99,
	D = 100,
	E = 101,
	F = 102,
	G = 103,
	H = 104,
	I = 105,
	J = 106,
	K = 107,
	L = 108,
	M = 109,
	N = 110,
	O = 111,
	P = 112,
	Q = 113,
	R = 114,
	S = 115,
	T = 116,
	U = 117,
	V = 118,
	W = 119,
	X = 120,
	Y = 121,
	Z = 122,
	LeftCurlyBracket = 123,
	Pipe = 124,
	RightCurlyBracket = 125,
	Tilde = 126,
	Numlock = 300,
	CapsLock = 301,
	ScrollLock = 302,
	RightShift = 303,
	LeftShift = 304,
	RightControl = 305,
	LeftControl = 306,
	RightAlt = 307,
	LeftAlt = 308,
	LeftCommand = 310,
	LeftApple = 310,
	LeftWindows = 311,
	RightCommand = 309,
	RightApple = 309,
	RightWindows = 312,
	AltGr = 313,
	Help = 315,
	Pr = 316,
	SysReq = 317,
	Break = 318,
	Menu = 319,
	Mouse1 = 323,
	Mouse2 = 324,
	Mouse3 = 325,
	Mouse4 = 326,
	Mouse5 = 327,
	Mouse6 = 328,
	Mouse7 = 329,
	Unbound = 10000
};

struct configHotkeys {
	std::string config_name;
	KeyCode hotkey;
};


enum class QueryTriggerInteraction
{
	// Token: 0x0400005B RID: 91
	UseGlobal,
	// Token: 0x0400005C RID: 92
	Ignore,
	// Token: 0x0400005D RID: 93
	Collide
};

enum class layer : uint32_t {
	Default = 0,
	TransparentFX = 1,
	Ignore_Raycast = 2,
	Reserved1 = 3,
	Water = 4,
	UI = 5,
	Reserved2 = 6,
	Reserved3 = 7,
	Deployed = 8,
	Ragdoll = 9,
	Invisible = 10,
	AI = 11,
	PlayerMovement = 12,
	Vehicle_Detailed = 13,
	Game_Trace = 14,
	Vehicle_World = 15,
	World = 16,
	Player_Server = 17,
	Trigger = 18,
	Player_Model_Rendering = 19,
	Physics_Projectile = 20,
	Construction = 21,
	Construction_Socket = 22,
	Terrain = 23,
	Transparent = 24,
	Clutter = 25,
	Debris = 26,
	Vehicle_Large = 27,
	Prevent_Movement = 28,
	Prevent_Building = 29,
	Tree = 30,
	Unused2 = 31
};

enum class Layers
{
	Terrain = 8388608,
	World = 65536,
	Ragdolls = 512,
	Construction = 2097152,
	ConstructionSocket = 4194304,
	Craters = 1,
	GameTrace = 16384,
	Trigger = 262144,
	VehiclesDetailed = 8192,
	RainFall = 1101070337,
	Deploy = 1235288065,
	DefaultDeployVolumeCheck = 537001984,
	BuildLineOfSightCheck = 2097152,
	ProjectileLineOfSightCheck = 2162688,
	ProjectileLineOfSightCheckTerrain = 10551296,
	MeleeLineOfSightCheck = 2162688,
	EyeLineOfSightCheck = 2162688,
	EntityLineOfSightCheck = 1218519041,
	PlayerBuildings = 18874624,
	PlannerPlacement = 161546496,
	Solid = 1218652417,
	VisCulling = 10551297,
	AltitudeCheck = 1218511105,
	HABGroundEffect = 1218511105,
	AILineOfSight = 1218519297,
	DismountCheck = 1486946561,
	AIPlacement = 278986753,
	WheelRay = 1235321089,
};


enum class EventType
{
	MouseDown = 0,
	MouseUp = 1,
	MouseMove = 2,
	MouseDrag = 3,
	KeyDown = 4,
	KeyUp = 5,
	ScrollWheel = 6,
	Repaint = 7,
	Layout = 8,
	DragUpdated = 9,
	DragPerform = 10,
	DragExited = 15,
	Ignore = 11,
	Used = 12,
	ValidateCommand = 13,
	ExecuteCommand = 14,
	ContextClick = 16,
	MouseEnterWindow = 20,
	MouseLeaveWindow = 21
};

//std::wstring			   get_name(uintptr_t object);

template<typename T>
class laddy_list {
public:
	T get(uint32_t idx)
	{
		const auto internal_list = reinterpret_cast<uintptr_t>(this + 0x20);
		return *reinterpret_cast<T*>(internal_list + idx * sizeof(T));
	}

	T get_value(uint32_t idx)
	{
		const auto list = *reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x10);
		const auto internal_list = list + 0x20;
		return *reinterpret_cast<T*>(internal_list + idx * sizeof(T));
	}

	const uint32_t get_size() { return *reinterpret_cast<uint32_t*>((uintptr_t)this + 0x18); }
};


namespace functions
{
	template <typename T>
	std::ptrdiff_t get_entity_index_in_list(const std::vector<T>& list, uintptr_t ent) {
		auto it = std::find_if(list.begin(), list.end(), [ent](const T& item) {
			return item.entity == ent;
			});

		if (it != list.end()) {
			return std::distance(list.begin(), it);
		}
		else {
			return -1; // Not found
		}
	}
	template <typename T>
	void remove_old_entries(std::vector<T>& arr, float current_time, float offset = 3000) {
		arr.erase(std::remove_if(arr.begin(), arr.end(), [current_time, offset](const T& item) {
			return item.last_check <= current_time - offset;
			}), arr.end());
	}
	std::string				format_time(float seconds_float);
	bool					WorldToScreen(const Vector3& EntityPos, Vector2& ScreenPos);
	bool					getPositionFromView(Vector3& out, Matrix4x4 m);
	char* memstr(char* haystack, const char* needle, int size);
	uintptr_t				scan_for_camera_list();
	int						strCmp(const char* s1, const char* s2);
	size_t					my_strlen(const char* str);
	bool					update_view_matrix();
	Vector3					get_camera_pos();
	Vector3					get_mouse_pos();
	int						get_screen_width();
	int						get_screen_height();
	std::string				ws2s(const std::wstring& wstr);
	std::wstring			s2ws(const std::string& str);
	std::wstring			read_unicode(uint64_t address, std::size_t size);
	std::string				read_ascii(uint64_t address, std::size_t size = 0x50);
	const char* read_ascii_alt(uint64_t address);
	bool					get_key_down(KeyCode key);
	bool					get_key_up(KeyCode key);
	bool					get_key(KeyCode key);
	bool				    get_mouse_button(int* button);
	void					ignore_layer_collision(layer the_layer, layer ignore_layer, bool should_collide);
	bool					line_of_sight(Vector3 start, Vector3 end, Layers layer_mask, float padding = 0);
	bool					is_visible(Vector3 start, Vector3 end);
	float					clamp(float value, float min, float max);
	float					mathf_atan2(float y, float x);
	uintptr_t get_texture(uintptr_t sprite);
	


	float					mathf_abs(float f);
	float					mathf_max(float a, float b);
	laddy_list<uintptr_t>*  get2renderers(uintptr_t multimesh);
	uintptr_t				get_material(uintptr_t renderer);
	uintptr_t				get_shader(uintptr_t material);
	uintptr_t				find(const char* shader);
	void					set_shader(uintptr_t material, uintptr_t shader);
	void					set_color(uintptr_t material, const char* name, Vector4 color);
	uintptr_t				get_current();
	EventType				get_event_type(uintptr_t current);
	KeyCode					get_keyCode(uintptr_t current);
	uintptr_t				get_draw_skin();
	uintptr_t				get_white_texture();
	void					set_draw_font_size(uintptr_t label, int size);
	void					set_draw_color(Vector4 color);
	void					set_draw_alignment(uintptr_t label, uintptr_t value);
	void					create(uintptr_t self, const char* shader);
	void					dont_destroy_on_load(uintptr_t target);
	void					add_component(uintptr_t self, uintptr_t componentType);
	void					draw_texture(Vector4 position, uintptr_t texture);
	void					outline_box(Vector2 position, Vector2 size, Vector4 color, float girth = 2.f);
	void					fill_box(Vector4 position, Vector4 color);
	void					draw_pixel(int x, int y);
	void					line(Vector2 point1, Vector2 point2, Vector4 color);
	void					draw_triangle(Vector2 point1, Vector2 point2, Vector2 point3, Vector4 color);
	void					draw_outline_circle(int centerX, int centerY, int radius, Vector4 color);
	void					label(Vector4 position, std::string text, Vector4 color, bool centered = false, float font_size = 12);
	void					outline(Vector4 position, std::string text, Vector4 color, bool centered = false, float font_size = 12);
	void					ddraw_get();
	uintptr_t				CreatePrefab(const char* prefab, bool active = true);
	void					Destroy(uintptr_t object);
	float					get_time();
	float					get_deltaTime();
	float					random_range(float min, float max);
	bool					physics_checkcapsule(Vector3 start, Vector3 end, float radius, int layer_mask, QueryTriggerInteraction querytriggerinteraction);
}

extern std::map<std::string, std::string> weapon_names;
extern std::map<KeyCode, std::string> keyCodeToString;

namespace vctr {
	class toolCupboard {
	public:
		float last_check;
		uintptr_t entity;
		Vector3 position;
		bool is_authed;
		std::vector<authList> player_list;
	};

	class turrets {
	public:
		float last_check;
		uintptr_t entity;
		Vector3 position;
		bool is_authed;
		std::vector<authList> player_list;
	};

	class hackableCrate {
	public:
		float last_check;
		uintptr_t entity;
		Vector3 position;
		float time;
	};

	class otherEntity {
	public:
		float last_check;
		uintptr_t entity;
		Vector3 position;
		Vector4 color;
		std::string name;
	};
}

namespace rust
{
	

	class Client
	{
	public:
		std::string		get_ip();
		int				get_port();
	};

	class World
	{
	public:
		std::string		get_url();
		unsigned int    get_mapsize();
		unsigned int    get_seed();
		bool			is_custom();
	};

	Client* get_client();
	World* get_world();

	class Transform
	{
	public:
		Vector3				get_position();
		void				set_position(Vector3 value);
	};

	class BaseEntity
	{
	public:
		std::wstring		get_name();
		std::string			get_prefab_name();
		Vector3				get_visual_position(uintptr_t object_class);
		Transform*			get_transform();
		void				server_rpc(const char*);
		static void			server_rpc(uintptr_t, const char*);
		void				send_signal_broadcast(int signal, const char* arg = "");
	};

	class BaseCombatEntity : public BaseEntity
	{
	public:
		float				get_health();
		float				get_max_health();
		int					get_life_state();
	};

	class PlayerWalkMovement
	{
	public:
		void				set_ground_angle(float angle);
		void				set_ground_angle_new(float angle);
		void				set_ground_time(float time);
		void				set_jump_time(float time);
		void				set_land_time(float time);
		void				set_targetmovement(Vector3);
		void				set_capsule_center(float);
		void				flying(bool value);
		void				set_was_falling(bool value);
		void				set_previous_velocity(Vector3 value);
	};

	class PlayerInput
	{
	public:
		Vector2				get_view_angles();
		void				set_view_angles(Vector2 angle);
	};

	class ItemDefinition : public BaseEntity
	{
	public:
		int					get_itemid();
		std::wstring		get_shortname();

	};

	class BaseProjectile : public BaseEntity
	{
	public:
		class Magazine : public BaseEntity
		{
		public:
			ItemDefinition* get_ammotype();
		};

		Magazine* get_primaryMagazine();
	};

	class Item : public BaseEntity
	{
	public:
		int					get_amount();
		int					get_uid();
		uintptr_t			get_sprite();
		int32_t				get_definition_id();
		std::wstring		get_name();
		Vector3				get_visual_position(uintptr_t);
		bool				get_recoil_min_max(Vector2& pitch_bounds, Vector2& yaw_bounds);
		void 				fakefire();
		BaseEntity*			get_held_entity();
		void				recoilControl(float value);
		void				set_always_eoka(float);
		void				set_aim_sway(float);
		void				set_aim_sway_speed(float);
		void				set_aim_cone(float);
		void				set_aim_cone_curve_scale(float);
		void				set_hip_aim_cone(float);
		void				set_aim_cone_penalty_max(float);
		void				set_aim_cone_penalty_per_shot(float);
		bool				is_gun();
	};

	class StashContainer
	{
	public:
		bool is_hidden();
	};

	class ItemContainer
	{
	public:
		Item*	  get_item(int);
		uintptr_t get_itemlist();
	};

	class PlayerInventory
	{
	public:
		ItemContainer* get_belt();
		ItemContainer* get_wear();
	};

	class TeamMember : public BaseEntity
	{
	public:
		long				get_uid();
	};

	class PlayerTeam
	{
	public:
		std::vector<long>	get_members();
		long				get_leader();
	};

	class entity_base {
	public:
		std::string name;
		//uintptr_t transform;
		//int networkid;
		float r;
		float g;
		float b;
		uintptr_t entity;
		Vector3 position;
	};

	class PlayerModel
	{
	public:
		Vector3				get_position(); //0x218
		Vector3				get_velocity();
		uintptr_t			get_multimesh();
		bool				is_npc();
		void				set_position(Vector3);
	};

	class ModelState
	{
	public:
		void				set_flying(bool value);
		void				remove_flag(int flag);
		void				add_flag(int flag);
		bool				has_flag(int flag);
	};



	class PlayerEyes : public BaseEntity
	{
	public:
		Vector3             get_viewoffset();
		Vector3				get_position();
		Vector3				get_bodyforward();
		Vector3				get_bodyright();
		void				set_body_rotation(Vector4 value);
		Vector4				get_body_rotation();
		Vector4				get_rotation();
	};

	template<typename T>
	class list {
	public:
		T get(uint32_t idx)
		{
			const auto internal_list = reinterpret_cast<uintptr_t>(this + 0x20);
			return *reinterpret_cast<T*>(internal_list + idx * sizeof(T));
		}

		T get_value(uint32_t idx)
		{
			const auto list = *reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x10);
			const auto internal_list = list + 0x20;
			return *reinterpret_cast<T*>(internal_list + idx * sizeof(T));
		}

		T operator[](uint32_t idx) { return get(idx); }

		const uint32_t get_size() { return *reinterpret_cast<uint32_t*>((uintptr_t)this + 0x18); }

		template<typename F>
		void for_each(const F callback)
		{
			for (auto i = 0; i < get_size(); i++) {
				auto object = this->get(i);
				if (!object)
					continue;
				callback(object, i);
			}
		}
	};

	class UIBlackoutOverlay
	{
	public:
		void set_alpha(float value);
	};

	UIBlackoutOverlay* get_overlay(int overlay);

	class BasePlayer : public BaseCombatEntity
	{
	public:
		bool				is_mounted();
		std::wstring		get_username();
		long				get_uid();
		int					get_active_weapon_id();
		PlayerWalkMovement* get_movement();
		PlayerInput*		get_input();
		PlayerInventory*	get_inventory();
		PlayerTeam*			get_team();
		ULONG				get_team_id();
		std::vector<Item*>  get_hotbar_items();
		std::vector<Item*>  get_clothing_items();
		PlayerModel*		get_model();
		ModelState*			get_modelstate();
		PlayerEyes*			get_eyes();
		Item*				get_held_item();
		bool				is_knocked();
		bool				is_sleeping();
		bool				is_visible(Vector3 end);
		Vector3				get_bone_position(int bone);
		Transform*			get_bone_transform(int bone);
		void				add_flag(int flag);
		bool				has_flag(int flag);
		bool				is_bot();
		static void			server_rpcs(uintptr_t, const char*, const char*, bool);
		uintptr_t			object;
	};

	inline std::array<int, 20> valid_bones = {
		1, 2, 3, 5, 6, 14, 15, 17, 18, 21, 23, 24, 25, 26, 27, 48, 55, 56, 57, 76
	};
	 struct box_bounds {
		float left, right, top, bottom;
		bool onscreen = false;

		bool empty() {
			if (this->left == 0 && this->right == 0 && this->top == 0 && this->bottom == 0)
				return true;

			if (this->left == FLT_MAX || this->right == FLT_MIN || this->top == FLT_MAX || this->bottom == FLT_MIN)
				return true;

			return false;
		}
		static box_bounds null() {
			return { 0, 0, 0, 0 };
		}
	};

	inline box_bounds get_bounds(rust::BasePlayer* player, float expand = 0) {
		box_bounds ret = { FLT_MAX, FLT_MIN, FLT_MAX, FLT_MIN };

		for (auto j : valid_bones) {
			auto transform = player->get_bone_transform(j);
			if (transform) {
				Vector2 sc;
				auto world_pos = transform->get_position();

				if (j == 48)
					world_pos.y += 0.2f;

				if (functions::WorldToScreen(world_pos, sc)) {
					Vector2 bone_screen = sc;

					if (bone_screen.x < ret.left)
						ret.left = bone_screen.x;
					if (bone_screen.x > ret.right)
						ret.right = bone_screen.x;
					if (bone_screen.y < ret.top)
						ret.top = bone_screen.y;
					if (bone_screen.y > ret.bottom)
						ret.bottom = bone_screen.y;
					ret.onscreen = true;
				}
			}
		}

		if (ret.left == FLT_MAX)
			return box_bounds::null();
		if (ret.right == FLT_MIN)
			return box_bounds::null();
		if (ret.top == FLT_MAX)
			return box_bounds::null();
		if (ret.bottom == FLT_MIN)
			return box_bounds::null();

		ret.left -= expand;
		ret.right += expand;
		ret.top -= expand;
		ret.bottom += expand;

		return ret;
	};

	class LocalPlayer : BasePlayer {
	public:
		static BasePlayer*  Entity();
		static void			ConsoleMessage(const char* message);
	};

	struct Ray {
		Vector3 origin;
		Vector3 dir;
		Ray(Vector3 o, Vector3 d) {
			origin = o;
			dir = d;
		}
	};

	class BuildingPrivelidge
	{
	public:
		std::vector<authList> get_authorized();
		bool				  is_authorized(std::vector<authList> authlist);
		float				  get_upkeep();
	};

	class SamSite
	{
	public:
		bool				  is_online();
	};

	class AutoTurret
	{
	public:
		std::vector<authList> get_authorized();
		bool				  is_authorized(std::vector<authList> authlist);
		bool				  is_online();
	};

	class HackableLockedCrate
	{
	public:
		float get_time();
	};

	class TOD_NightParameters
	{
	public:
		void				set_light_intensity(float intensity);
		void				set_reflection_multiplier(float multiplier);
		void				set_ambient_multiplier(float multiplier);
	};

	class TOD_DayParameters
	{
	public:
		void				set_ambient_multiplier(float multiplier);
	};

	class TOD_Sky
	{
	public:
		static TOD_Sky* get_instance();
		TOD_NightParameters* get_night();
		TOD_DayParameters* get_day();
	};

	class DownloadHandler
	{
	public:
		il2cpp::il2cppstring* GetText();
	};

	class UnityWebRequest
	{
	public:
		static UnityWebRequest* Get(il2cpp::il2cppstring* string);
		uintptr_t				SendWebRequest();
		DownloadHandler*		GetDownloadHandler();
		bool					set_header(il2cpp::il2cppstring* name, il2cpp::il2cppstring* value);
	};

	class BuildingBlock
	{
	public:
		bool				can_afford(int grade, BasePlayer* player, int buildingskin);
		bool				can_upgrade(int grade, BasePlayer* player, int buildingskin);
		void				upgrade(int grade, BasePlayer* player, int buildingskin);
		int					get_grade();
	};
}

class explosion_data
{
public:
	std::chrono::steady_clock::time_point start;
	rust::BaseEntity* entity;
	Vector3 position;
	std::string class_name;
	std::string prefab_name;
};

struct player_local {
	std::string name;
	std::string weapon_name = "";
	uintptr_t player;
	rust::PlayerTeam* team;
	rust::Item* weapon;
	ULONG teamid;
	long userid;
	float last_check;
	bool found = false;
	int life_state;
	rust::PlayerWalkMovement* movement;
	rust::PlayerEyes* eyes;
};


struct playerEnt {
	std::string name;
	float health;
	bool teammate;
	rust::PlayerTeam* team;
	rust::Item* weapon;
	ULONG teamid;
	long userid;
	bool npc;
	Vector3 position;
	bool sleeping;
	bool knocked;
	int life_state;
	bool did_chams = false;
	bool render_check = false;
	float check_count = 0;
	rust::box_bounds bounds;
	uintptr_t entity;
	float last_check;
	bool bot;
	bool found;
	bool is_connected;
};

namespace variables
{
	extern rust::BasePlayer* local_player;
	extern Vector3				local_position;
	extern Matrix4x4			view_matrix;
	extern long					local_userid;
	extern int					screen_width;
	extern int					screen_height;
	extern int					image_width;
	extern int					image_height;
	//extern ID3D11ShaderResourceView* resource_view;
	//extern ImDrawList* draw_list;
	//extern ImFont* smallest_pixel_7_font;
	//extern std::vector<player_data> player_data;
	//extern std::vector<explosion_data> explosion_data;
	extern Vector2				mouse_position;
	extern Vector2				mouse_position_backup;
	extern bool					loaded;
	extern uintptr_t			draw_label;
	extern uintptr_t			white_texture;
	extern uintptr_t			camera_list;
	extern uintptr_t			client_entities;
	extern uintptr_t			debug_camera_address;
	extern std::vector<rust::entity_base> entity_draw;
	extern std::vector<vctr::otherEntity> helis;

	extern std::vector<playerEnt> players;
	extern player_local lcp;


	extern std::vector<vctr::toolCupboard> tc_list;
	extern std::vector<vctr::turrets> turret_list;
	extern std::vector<vctr::hackableCrate> hackablecrate_list;
	//extern chaiscript::ChaiScript chai;
	//extern ImFont* smallestpixel7;
}

class PlayerNameID {
public:
	std::string get_username() {
		il2cpp::il2cppstring* player_name = *reinterpret_cast<il2cpp::il2cppstring**>((uintptr_t)this + 0x10);
		if (reinterpret_cast<uintptr_t>(player_name)) {
			return functions::ws2s(player_name->buffer);
		}
		else {
			return "!";
		}
	}

	unsigned long get_user_id()
	{
		return *reinterpret_cast<unsigned long*>((uintptr_t)this + 0x18);
	}
};