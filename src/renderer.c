#include "renderer.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "math3d.h"

#define PHI 1.618033988749895f

int compare_edges(const void* a, const void* b) {
    edge_t* ea = (edge_t*)a;
    edge_t* eb = (edge_t*)b;
    if (ea->depth > eb->depth) return -1;
    if (ea->depth < eb->depth) return 1;
    return 0;
}

mat4_t create_view_matrix(camera_t camera) {
    vec3_t forward = vec3_normalize(vec3_from_cartesian(
        camera.target.x - camera.position.x,
        camera.target.y - camera.position.y,
        camera.target.z - camera.position.z
    ));

    vec3_t right = vec3_normalize(vec3_from_cartesian(
        forward.y * camera.up.z - forward.z * camera.up.y,
        forward.z * camera.up.x - forward.x * camera.up.z,
        forward.x * camera.up.y - forward.y * camera.up.x
    ));

    vec3_t up = vec3_from_cartesian(
        right.y * forward.z - right.z * forward.y,
        right.z * forward.x - right.x * forward.z,
        right.x * forward.y - right.y * forward.x
    );

    mat4_t view = mat4_identity();
    view.m[0] = right.x; view.m[4] = right.y; view.m[8] = right.z;
    view.m[12] = -(right.x * camera.position.x + right.y * camera.position.y + right.z * camera.position.z);

    view.m[1] = up.x; view.m[5] = up.y; view.m[9] = up.z;
    view.m[13] = -(up.x * camera.position.x + up.y * camera.position.y + up.z * camera.position.z);

    view.m[2] = -forward.x; view.m[6] = -forward.y; view.m[10] = -forward.z;
    view.m[14] = forward.x * camera.position.x + forward.y * camera.position.y + forward.z * camera.position.z;

    view.m[3] = 0; view.m[7] = 0; view.m[11] = 0; view.m[15] = 1;

    return view;
}

mat4_t create_projection_matrix(camera_t camera) {
    float f = 1.0f / tanf(camera.fov * 0.5f);
    float range_inv = 1.0f / (camera.near - camera.far);

    mat4_t proj = mat4_identity();
    proj.m[0] = f / camera.aspect;
    proj.m[5] = f;
    proj.m[10] = (camera.near + camera.far) * range_inv;
    proj.m[11] = -1.0f;
    proj.m[14] = 2.0f * camera.near * camera.far * range_inv;
    proj.m[15] = 0.0f;

    return proj;
}

vec3_t project_vertex(vec3_t vertex, mat4_t model_matrix, mat4_t view_matrix,
                     mat4_t projection_matrix, viewport_t viewport) {
    vec3_t world_pos = mat4_transform_point(model_matrix, vertex);
    vec3_t camera_pos = mat4_transform_point(view_matrix, world_pos);

    float x = camera_pos.x;
    float y = camera_pos.y;
    float z = camera_pos.z;
    float w = 1.0f;

    float proj_x = projection_matrix.m[0] * x + projection_matrix.m[4] * y + projection_matrix.m[8] * z + projection_matrix.m[12] * w;
    float proj_y = projection_matrix.m[1] * x + projection_matrix.m[5] * y + projection_matrix.m[9] * z + projection_matrix.m[13] * w;
    float proj_z = projection_matrix.m[2] * x + projection_matrix.m[6] * y + projection_matrix.m[10] * z + projection_matrix.m[14] * w;
    float proj_w = projection_matrix.m[3] * x + projection_matrix.m[7] * y + projection_matrix.m[11] * z + projection_matrix.m[15] * w;

    if (fabsf(proj_w) > 1e-6f) {
        proj_x /= proj_w;
        proj_y /= proj_w;
        proj_z /= proj_w;
    }

    float screen_x = viewport.center_x + (proj_x * viewport.radius);
    float screen_y = viewport.center_y - (proj_y * viewport.radius);

    return vec3_from_cartesian(screen_x, screen_y, proj_z);
}

bool clip_to_circular_viewport(canvas_t* canvas, float x, float y, viewport_t viewport) {
    if (x < 0 || x >= canvas->width || y < 0 || y >= canvas->height) return false;

    float dx = x - viewport.center_x;
    float dy = y - viewport.center_y;
    float dist_sq = dx * dx + dy * dy;

    return dist_sq <= (viewport.radius * viewport.radius);
}

void draw_line_clipped(canvas_t* canvas, float x0, float y0, float x1, float y1,
                      float thickness, viewport_t viewport) {
    bool p0_inside = clip_to_circular_viewport(canvas, x0, y0, viewport);
    bool p1_inside = clip_to_circular_viewport(canvas, x1, y1, viewport);

    if (!p0_inside && !p1_inside) return;

    draw_line_f(canvas, x0, y0, x1, y1, thickness);
}

void render_wireframe(canvas_t* canvas, mesh_t* mesh, mat4_t model_matrix,
                     camera_t camera, viewport_t viewport) {
    if (!mesh || !mesh->vertices || !mesh->edges) return;

    mat4_t view_matrix = create_view_matrix(camera);
    mat4_t projection_matrix = create_projection_matrix(camera);

    vec3_t* projected = malloc(mesh->vertex_count * sizeof(vec3_t));
    for (int i = 0; i < mesh->vertex_count; i++) {
        projected[i] = project_vertex(mesh->vertices[i], model_matrix,
                                     view_matrix, projection_matrix, viewport);
    }

    edge_t* sorted_edges = malloc(mesh->edge_count * sizeof(edge_t));
    for (int i = 0; i < mesh->edge_count; i++) {
        sorted_edges[i] = mesh->edges[i];
        float depth0 = projected[mesh->edges[i].v0].z;
        float depth1 = projected[mesh->edges[i].v1].z;
        sorted_edges[i].depth = (depth0 + depth1) * 0.5f;
    }

    qsort(sorted_edges, mesh->edge_count, sizeof(edge_t), compare_edges);

    for (int i = 0; i < mesh->edge_count; i++) {
        edge_t edge = sorted_edges[i];
        vec3_t p0 = projected[edge.v0];
        vec3_t p1 = projected[edge.v1];

        if (edge.v0 < mesh->vertex_count && edge.v1 < mesh->vertex_count) {
            if (p0.z > -1.0f && p0.z < 1.0f && p1.z > -1.0f && p1.z < 1.0f) {
                draw_line_clipped(canvas, p0.x, p0.y, p1.x, p1.y, 1.0f, viewport);
            }
        }
    }

    free(projected);
    free(sorted_edges);
}

mesh_t* generate_soccer_ball() {
    mesh_t* mesh = malloc(sizeof(mesh_t));
    mesh->vertex_count = 60;
    mesh->vertices = malloc(mesh->vertex_count * sizeof(vec3_t));

    vec3_t face_centers[20];

    vec3_t ico_vertices[12] = {
        vec3_from_cartesian(0, 1, PHI),
        vec3_from_cartesian(0, -1, PHI),
        vec3_from_cartesian(0, 1, -PHI),
        vec3_from_cartesian(0, -1, -PHI),
        vec3_from_cartesian(1, PHI, 0),
        vec3_from_cartesian(-1, PHI, 0),
        vec3_from_cartesian(1, -PHI, 0),
        vec3_from_cartesian(-1, -PHI, 0),
        vec3_from_cartesian(PHI, 0, 1),
        vec3_from_cartesian(-PHI, 0, 1),
        vec3_from_cartesian(PHI, 0, -1),
        vec3_from_cartesian(-PHI, 0, -1)
    };

    for (int i = 0; i < 12; i++) {
        ico_vertices[i] = vec3_normalize(ico_vertices[i]);
    }

    int vertex_idx = 0;
    for (int i = 0; i < 12; i++) {
        mesh->vertices[vertex_idx++] = ico_vertices[i];
    }

    int face_triangles[20][3] = {
        {0,1,8}, {0,8,4}, {0,4,5}, {0,5,9}, {0,9,1},
        {2,3,11}, {2,11,5}, {2,5,4}, {2,4,10}, {2,10,3},
        {1,6,8}, {1,7,6}, {1,9,7}, {8,6,10}, {8,10,4},
        {9,5,11}, {9,11,7}, {6,7,3}, {6,3,10}, {7,11,3}
    };

    for (int i = 0; i < 20; i++) {
        vec3_t center = vec3_from_cartesian(0, 0, 0);
        for (int j = 0; j < 3; j++) {
            center.x += ico_vertices[face_triangles[i][j]].x;
            center.y += ico_vertices[face_triangles[i][j]].y;
            center.z += ico_vertices[face_triangles[i][j]].z;
        }
        center.x /= 3.0f;
        center.y /= 3.0f;
        center.z /= 3.0f;
        face_centers[i] = vec3_normalize(center);

        if (vertex_idx < mesh->vertex_count) {
            mesh->vertices[vertex_idx++] = face_centers[i];
        }
    }

    for (int i = 0; i < 12 && vertex_idx < mesh->vertex_count; i++) {
        for (int j = i + 1; j < 12 && vertex_idx < mesh->vertex_count; j++) {
            vec3_t midpoint = vec3_from_cartesian(
                (ico_vertices[i].x + ico_vertices[j].x) * 0.5f,
                (ico_vertices[i].y + ico_vertices[j].y) * 0.5f,
                (ico_vertices[i].z + ico_vertices[j].z) * 0.5f
            );
            mesh->vertices[vertex_idx++] = vec3_normalize(midpoint);
        }
    }

    while (vertex_idx < mesh->vertex_count) {
        mesh->vertices[vertex_idx++] = vec3_from_cartesian(0, 0, 1);
    }

    mesh->edge_count = 200;
    mesh->edges = malloc(mesh->edge_count * sizeof(edge_t));
    int edge_idx = 0;

    for (int i = 0; i < mesh->vertex_count && edge_idx < mesh->edge_count; i++) {
        for (int j = i + 1; j < mesh->vertex_count && edge_idx < mesh->edge_count; j++) {
            float dx = mesh->vertices[i].x - mesh->vertices[j].x;
            float dy = mesh->vertices[i].y - mesh->vertices[j].y;
            float dz = mesh->vertices[i].z - mesh->vertices[j].z;
            float dist = sqrtf(dx*dx + dy*dy + dz*dz);

            if (dist < 0.85f) {
                mesh->edges[edge_idx].v0 = i;
                mesh->edges[edge_idx].v1 = j;
                mesh->edges[edge_idx].depth = 0.0f;
                edge_idx++;
            }
        }
    }

    mesh->edge_count = edge_idx;

    return mesh;
}

void free_mesh(mesh_t* mesh) {
    if (mesh) {
        free(mesh->vertices);
        free(mesh->edges);
        free(mesh);
    }
}
