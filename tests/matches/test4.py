# Dict Destructuring
def mapping(user_dict):
    match user_dict:
        case {"role": "admin", "name": name}:
            return f"Admin user: {name}"
        case {"role": "guest"}:
            return "Guest user"
        case {"id": user_id, **extra}:
            return f"User ID {user_id} with extra fields: {extra}"
        case _:
            return "Unknown structure"
