#include "Game.h"

// Johirul's code

void Game::Making_circle_shaped_boundary_for_car_that_it_wont_go_out_of_road() {
	//from shuvo
	float dx, dy, ang;
	ang = this->plr.getRotation();
	dx = speed * cosf(ang * pi / 180.f);
	dy = speed * sinf(ang * pi / 180.f);

	//from me
	FloatRect nextpos = plr.getGlobalBounds();
	nextpos.left += 10 * dx;
	nextpos.top += 10 * dy;

	bool move = false;



	for (const auto& circle : this->road) {

		Vector2f center = circle.getPosition();
		float radius = circle.getRadius();

		float closest_point_from_center_x_axis = max(nextpos.left, min(center.x, nextpos.left + nextpos.width));
		float closest_point_from_center_y_axis = max(nextpos.top, min(center.y, nextpos.top + nextpos.height));


		float distance_x_axis_from_center = center.x - closest_point_from_center_x_axis;
		float distance_y_axis_from_center = center.y - closest_point_from_center_y_axis;
		float distance_square = (distance_x_axis_from_center * distance_x_axis_from_center) + (distance_y_axis_from_center * distance_y_axis_from_center);


		if (distance_square < (radius * radius)) {
			move = true;
			break; // Found a valid road circle, no need to check others
		}
	}

	if (move) {
		plr.move(dx, dy);
	}
	else {
		speed = 0;
	}
}



void Game::minimap_zoom() {
	if (this->ev.mouseButton.button == Mouse::Left) {
		Vector2i mouse_position = Mouse::getPosition(*this->window);

		FloatRect map_area_to_click;
		if (this->minimap_domain_expantion) {
			map_area_to_click = FloatRect(this->window->getSize().x / 2.f, 0.f, this->window->getSize().x / 2.f, this->window->getSize().y);
		}
		else {
			map_area_to_click = FloatRect(this->window->getSize().x * 0.75f, 0.f,
				this->window->getSize().x * 0.25f, this->window->getSize().y * 0.25f);
		}

		if (map_area_to_click.contains(static_cast<Vector2f>(mouse_position))) {

			this->minimap_domain_expantion = !this->minimap_domain_expantion;

			
			/*this->minimap_view.setSize(this->right_most_x-this->left_most_x, this->right_most_y-this->left_most_y);*/
			/*this->minimap_view.setCenter(Vector2f((this->right_most_x + this->left_most_x) / 2, (this->right_most_y + this->left_most_y) / 2));*/

			if (this->minimap_domain_expantion) {
				/*minimap_view.setCenter( )*/
				this->minimap_view.setSize(this->right_most_x - this->left_most_x + 200, this->right_most_y - this->left_most_y+200);
				this->minimap_view.setCenter(Vector2f((this->right_most_x + this->left_most_x) / 2, (this->right_most_y + this->left_most_y) / 2));

				/*this->minimap_view.zoom(1.2f);*/
			}
			else {
				this->minimap_view.setSize(this->desktopMode.width, this->desktopMode.height);
				this->minimap_view.zoom(0.5f);
			}
		}
	}
}
