
#include "scene2D/manager_2D.hh"
#include "scene2D/scene2D.hh"

#include "scene/manager_quad.hh"



QID ManagerQuad::
new_qid()
{
    if(qid_index == 0)
        throw std::runtime_error("Ran out of new QIDs");
    
    return qid_index++;
}



Quad* ManagerQuad::find_quad_qid(QID qid)
{
    for(QuadO& quado : quados)
    {
        if(qid == quado.quad.qid)
            return &quado.quad;
    }

    return nullptr;
}

Quad* ManagerQuad::find_quad_oid(OID oid)
{
    for(QuadO& quado : quados)
    {
        if(oid == quado.tag.oid)
            return &quado.quad;
    }

    return nullptr;
}




bool ManagerQuad::
try_new_quad_grab(window::InputEvent& event, TagO sampled_tag)
{
    bool is_quad_grab_click = event.is_mouse_button() && event.mouse_button.is_left_down() && event.modifiers.is_ctrl();

    if( sampled_tag.is_quad()   &&
        is_quad_grab_click          )
    {
        state.capturing_tag = sampled_tag;
        return true;
    }

    state.capturing_tag = TagO();
	return false;
}


bool ManagerQuad::
try_release_quad(window::InputEvent& event, TagO sampled_tag)
{
    // Click on non-capturing object
    bool is_quad_release_click = event.is_mouse_button() && event.mouse_button.is_left_down();
    if( is_quad_release_click &&
        sampled_tag != state.capturing_tag  )
    {
        state.capturing_tag = TagO();
        return true;
    }

    // Esc
    bool quad_release_esc = event.is_keystroke() && event.keystroke.is(Key::Esc);
    if(quad_release_esc)
    {
        state.capturing_tag = TagO();
		return true;
    }


    return false;
}



InputResponse ManagerQuad::
handle_event(window::InputEvent& event, Manager2D& manager_2D)
{
    auto* capturing_quad = find_quad_oid(state.capturing_tag.oid);
	if(capturing_quad == nullptr)   return InputResponse();

	if(capturing_quad->is_scene2D())
	{
		Scene2D* scene_p = manager_2D.search_scene_storage_2D(capturing_quad->sid_2D);
		
		if(event.is_mouse_scroll())
			scene_p->handle_scroll(event);
		if(event.is_mouse_button())
			scene_p->handle_pointer_click(event);
		if(event.is_mouse_movement())
			scene_p->handle_pointer_move(event);

        return {InputResponse::MOUSE_GRAB};
	}

    return InputResponse();
}
