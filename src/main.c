#include "neighborhood_search.h"
#define INITTEMP 273
#define SOURCETEMP 373

static void colorFromTemp(float xpos, float color[3])
{
	float v1 = 3.5 * (xpos - 0.7);
	float v2 = 1.25 * xpos;
	float v3 = fminf(0.5, xpos) * 2.0;

	color[0] = -v1 * v1 + 1.0f;
	color[1] = 6.0f * v2 * v2 * (1.0f - v2);
	color[2] = 5.5f * v3 * (1.0f - v3) * (1.0f - v3);

	// alternative: classical jet colormap
	// color[0] = 1.5 - 4.0 * fabs(v - 0.75);
	// color[1] = 1.5 - 4.0 * fabs(v - 0.5 );
	// color[2] = 1.5 - 4.0 * fabs(v - 0.25);
}

void fillDataSem3(GLfloat(*data)[8], GLfloat(*coord)[2], int nPoints)
{
	//float rmax = 100.0 * sqrtf(2.0f);
	for (int i = 0; i < NPTS; i++) {
		data[i][0] = rand() * 200.0 / RAND_MAX - 100.0; // x (rand between -100 and 100)
		data[i][1] = rand() * 200.0 / RAND_MAX - 100.0; // y (rand between -100 and 100)
		coord[i][0] = data[i][0];
		coord[i][1] = data[i][1];
		//float r = sqrt(data[i][0] * data[i][0] + data[i][1] * data[i][1]);
		data[i][2] = 0; //fixed particles for Seminar 3
		data[i][3] = 0; //fixed particles for Seminar 3
		//data[i][2] = rand() * 2.0 / RAND_MAX - 1.0; //Random starting speed
		//data[i][3] = rand() * 2.0 / RAND_MAX - 1.0; //Random starting speed

		colorFromTemp(INITTEMP, &data[i][4]); // fill color
		data[i][7] = 0.8f; // transparency
	}
}

int main()
{

	GLfloat(*data)[8] = malloc(sizeof(data[0]) * NPTS);
	CHECK_MALLOC(data);
	GLfloat(*coord)[2] = malloc(sizeof(coord[0]) * NPTS);
	CHECK_MALLOC(coord);

	// Seed the random
	time_t seed = time(NULL);
	printf(" %u \n", seed);
	srand(seed);
	fillDataSem3(data, coord, NPTS);

	//No need to update neighborhood because particules have zero speed
	neighborhood** nhList = create_neighborhood(data, coord, 1, 1, NPTS, USE_CELLS, USE_IMPROVED_METHOD, RADIUS_ALGORITHM, SEARCH_NEIGHBORHOOD, USE_VERLET, USE_THREADS,NTHREADS, seed);



	double scale = 0.008;
	bov_window_t* window = bov_window_new(1024, 780, "ANM Project: SPH");
	bov_window_set_color(window, (GLfloat[]) { 0.9f, 0.85f, 0.8f, 0.0f });
	bov_points_t* particles = bov_particles_new(data, NPTS, GL_STATIC_DRAW);
	bov_points_set_width(particles, 0.02);
	bov_points_set_outline_width(particles, 0.0025);
	bov_points_scale(particles, (GLfloat[2]) { scale, scale });
	bov_points_set_pos(particles, (GLfloat[2]) { 0.0, -0.1 });

	while (!bov_window_should_close(window)) {
		bov_particles_draw(window, particles, 0, BOV_TILL_END);
		bov_window_update(window);
	}
	//bov_window_update_and_wait_events(window);

	bov_window_delete(window);
	bov_points_delete(particles);
	for (int i = 0; i < MAX_ITER; i++)
		neighborhood_delete(nhList[i], NPTS);
	free(nhList);
	free(coord);
	free(data);

	//how_equivalence_of_major_algorithm_combination(0);

	return EXIT_SUCCESS;
}




